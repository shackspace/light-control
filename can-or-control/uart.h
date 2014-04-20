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

#define UART_FIFOSIZE_IN	100
#define UART_FIFOSIZE_OUT	80

// FIFO-Objekte und Puffer für die Ein- und Ausgabe 

extern fifo_t uart_infifo;
extern fifo_t uart_outfifo;


extern void uart_init(void);

extern uint8_t uart_putc (const uint8_t);
extern void uart_write(const char*);

#define DIRECTION_CONTROL	0

#endif /* _UART_H_ */
