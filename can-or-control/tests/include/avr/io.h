#ifndef _AVR_IO_H_
#define _AVR_IO_H_

#define E2END        0x3FF
#define __AVR_ATmega328P__

#define pgm_read_byte(X) *data

uint8_t SREG;
#define cli()

uint8_t DDRA;
uint8_t DDRB;
uint8_t DDRC;
uint8_t DDRD;
uint8_t PORTA;
uint8_t PORTB;
uint8_t PORTC;
uint8_t PORTD;

#define PD2 2
#define _BV(bit) 	   (1 << (bit))


#endif
