#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <avr/io.h>

#include "greatest.h"

uint8_t SREG = 0;
uint8_t DDRA = 0;
uint8_t DDRB = 0;
uint8_t DDRC = 0;
uint8_t DDRD = 0;
uint8_t PORTA = 0;
uint8_t PORTB = 0;
uint8_t PORTC = 0;
uint8_t PORTD = 0;

uint8_t key_state = 0;


void eeprom_read_block(uint8_t tmp, uint8_t tmp2, uint8_t tmp3)
{
	return 0;
}




