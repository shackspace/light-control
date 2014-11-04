#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <avr/io.h>

#include "greatest.h"

#include "../fifo.h"

#include "../uart.h"

#define BUFSIZE_OUT UART_FIFOSIZE_OUT

uint8_t outbuf[BUFSIZE_OUT];
fifo_t uart_outfifo;




void uart_init()
{
    // FIFOs für Ein- und Ausgabe initialisieren 
    fifo_init (&uart_outfifo, outbuf, BUFSIZE_OUT);
}


uint8_t uart_putc (const uint8_t c)
{
    uint8_t ret = fifo_put (&uart_outfifo, c);
    return ret;
};

uint8_t uart_size_free_buffer_tx(void)
{
	return (uart_outfifo.size - uart_outfifo.count);
}

