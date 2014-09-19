/*----------------------------------------------------------------------------
 Copyright:      Ulrich Escher  mailto: code@myvdr.de
 Author:         Ulrich Escher, Georg-Johann Lay(http://www.gjlay.de/pub/index.html)
 Remarks:        
 known Problems: none
 Version:        13.01.2012
 Description:    Human Machine Interface

------------------------------------------------------------------------------*/

#include <avr/io.h>
#include <avr/interrupt.h>

#include "config.h"
#include "uart.h"
#include "fifo.h"

/* set the UART baud rate and Fifosize*/
#ifndef BAUDRATE
  #define BAUDRATE UART_BAUDRATE
#endif

#define BUFSIZE_IN  UART_FIFOSIZE_IN
#define BUFSIZE_OUT UART_FIFOSIZE_OUT



// FIFO-Objekte und Puffer für die Ein- und Ausgabe 
uint8_t inbuf[BUFSIZE_IN];
fifo_t uart_infifo;

uint8_t outbuf[BUFSIZE_OUT];
fifo_t uart_outfifo;


void uart_init()
{
    uint8_t sreg = SREG;
    uint16_t ubrr = (uint16_t) ((uint32_t) F_CPU/(8UL*BAUDRATE) - 1);

    UBRRH = (uint8_t) (ubrr>>8);
    UBRRL = (uint8_t) (ubrr);

    // Interrupts kurz deaktivieren 
    cli();

    // UART Receiver und Transmitter anschalten, Receive-Interrupt aktivieren 
    // Data mode 8N1, asynchron 
    UCSRB = (1 << RXEN) | (1 << TXEN) | (1 << RXCIE) | (1 << TXCIE);
    UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);

    // Flush Receive-Buffer (entfernen evtl. vorhandener ungültiger Werte) 
    do
    {
        // UDR auslesen (Wert wird nicht verwendet) 
        UDR;
    }
    while (UCSRA & (1 << RXC));

    // Rücksetzen von Receive und Transmit Complete-Flags 
    UCSRA = (1 << RXC) | (1 << TXC) | (1 << U2X);

    // Global Interrupt-Flag wieder herstellen 
    SREG = sreg;

    // FIFOs für Ein- und Ausgabe initialisieren 
    fifo_init (&uart_infifo,   inbuf, BUFSIZE_IN);
    fifo_init (&uart_outfifo, outbuf, BUFSIZE_OUT);
}


// Empfangene Zeichen werden in die Eingabgs-FIFO gespeichert und warten dort 
ISR(USART_RXC_vect)
{
    fifo_put (&uart_infifo, UDR);
}
ISR(USART_TXC_vect)
{
  #if DIRECTION_CONTROL
    TX_NE
  #endif
}

// Ein Zeichen aus der Ausgabe-FIFO lesen und ausgeben 
// Ist das Zeichen fertig ausgegeben, wird ein neuer SIG_UART_DATA-IRQ getriggert 
// Ist die FIFO leer, deaktiviert die ISR ihren eigenen IRQ. 
ISR(USART_UDRE_vect)
{
    if (uart_outfifo.count > 0) {
        UDR = fifo_get (&uart_outfifo);
      #if DIRECTION_CONTROL
        TX_EN
      #endif
    } else {
        UCSRB &= ~(1 << UDRIE);
	}
}

uint8_t uart_putc (const uint8_t c)
{
    uint8_t ret = fifo_put (&uart_outfifo, c);
	
    UCSRB |= (1 << UDRIE);
	 
    return ret;
}

//----------------------------------------------------------------------------
//Ausgabe eines Strings
void uart_write(const char *str)
{
	while (*str)
	{
		uart_putc(*str++);
	}
}

