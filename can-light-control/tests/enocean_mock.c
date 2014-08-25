#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <avr/eeprom.h>
#include <avr/io.h>


#include "greatest.h"

#include "../config.h"

#include "../fifo.h"
#include "../enocean.h"
#include "../uart.h"
#include "../enocean.h"

void _delay_ms(uint8_t val);
void _delay_ms(uint8_t val){return;};
void let_set(uint8_t val);
void led_set(uint8_t val1, uint8_t val2){return;};
void reset_visualisation(void);
void reset_visualisation(void){return;};

uint8_t enocean_channel_storage[4] = {0,0,0,0};

void enocean_mock_init(void);
void enocean_mock_init(void)
{
	enocean_channel_storage[0] = 0;
	enocean_channel_storage[1] = 0;
	enocean_channel_storage[2] = 0;
	enocean_channel_storage[3] = 0;
	return;
}


bool enocean_check_uart_output(uint8_t channel, uint8_t state);

bool enocean_check_uart_output(uint8_t channel, uint8_t state)
{
	if ( fifo_get_count(&uart_outfifo) % 14 ) return false; 
	
	uint8_t fifo_read_tmp[14];	
	fifo_get_var (&uart_outfifo, fifo_read_tmp, 14);


	uint8_t packet_tmp[14] = {0xa5, 0x5a, 0x0b, 0x05, 
		((state+2) << 5 | 16),
		0, 0, 0, 0, 0, 0,
		(channel), 0x10, 0};

	for (uint8_t i = 2; i < 13; i++) {
		packet_tmp[13] += packet_tmp[i];
	}

	if ( memcmp(fifo_read_tmp, packet_tmp, 14) != 0 )
		return false;


return true;

}

uint8_t eeprom_read_byte(const uint8_t __p) __ATTR_PURE__
{
	uint8_t tmp_addr = __p - ENOCEAN_CHANNEL_EEPROM_STORE;
	return enocean_channel_storage[ tmp_addr ];
}

void eeprom_write_byte (uint8_t __p, uint8_t __value)
{
	uint8_t tmp_addr = __p - ENOCEAN_CHANNEL_EEPROM_STORE;
	enocean_channel_storage[ tmp_addr ] = __value;
	return 0;
}


