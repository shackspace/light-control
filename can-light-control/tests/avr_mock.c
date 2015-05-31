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

typedef union
{
	uint8_t type8[8];
	uint64_t type64;
} system_time_t;
volatile system_time_t system_time;


void eeprom_read_block(uint8_t tmp, uint8_t tmp2, uint8_t tmp3)
{
	return 0;
}

uint8_t reboot_flag = 0;
void set_reboot(void)
{
	reboot_flag = 1;
	return;
}
uint8_t get_reboot(void)
{
	return reboot_flag;
}



