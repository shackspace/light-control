/*----------------------------------------------------------------------------
 Copyright:      Ulrich Escher  mailto: code@myvdr.de
 Author:         Ulrich Escher, Georg-Johann Lay(http://www.gjlay.de/pub/index.html)
 Remarks:        
 known Problems: none
 Version:        13.01.2012
 Description:    UART

------------------------------------------------------------------------------*/

#ifndef _UART_H_
#define _UART_H_

#include <avr/io.h>
#include "fifo.h"

#ifndef UART_BAUDRATE
  #define UART_BAUDRATE	9600UL
#endif

#define UART_FIFOSIZE_IN	28
#define UART_FIFOSIZE_OUT	28

// FIFO-Objekte und Puffer für die Ein- und Ausgabe 

extern fifo_t uart_infifo;
extern fifo_t uart_outfifo;

extern volatile int8_t uart_frameing;

extern void uart_init(void);
extern void uart_main(void);

extern uint8_t uart_putc (const uint8_t);
extern void uart_write(const char*);

extern uint8_t uart_size_free_buffer_tx(void);
extern uint8_t uart_size_free_buffer_rx(void);

extern uint8_t uart_size_buffer_tx(void);
extern uint8_t uart_size_buffer_rx(void);

extern void uart_enable_tx(void);
extern void uart_disable_tx(void);

#define DIRECTION_CONTROL	0

#endif /* _UART_H_ */
