/*----------------------------------------------------------------------------
 Copyright:      Ulrich Escher  mailto: code@myvdr.de
 Author:         Ulrich Escher
 Remarks:        
 known Problems: none
 Version:        13.01.2012
                 13.04.2012 upgrade Channels
 Description:    enocean

 implement enocean

------------------------------------------------------------------------------*/

#ifndef _ENOCEAN_H_
#define _ENOCEAN_H_

#include "config.h"

#if USE_ENOCEAN

	#include <avr/io.h>
	#include <avr/pgmspace.h>
	#include <avr/eeprom.h>
	#include <string.h>
	
	#include "uart.h"

	#define ENOCEAN_CHANNEL_COUNT           4
	#define ENOCEAN_CHANNEL_OFFSET          140

	#define ENOCEAN_CHANNEL_EEPROM_STORE    60

	#define ENOCEAN_CHANNEL_STATUS  1
	#define ENOCEAN_CHANNEL_ACT     2
	#define ENOCEAN_CHANNEL_EEPROM  4

	#define ENOCEAN_CHANNEL_          0b10000000
	#define ENOCEAN_CHANNEL_SS        0b10010001
	#define ENOCEAN_CHANNEL_CS        0b10010000
	#define ENOCEAN_CHANNEL_SA        0b10100010
	#define ENOCEAN_CHANNEL_SE        0b11000100
	#define ENOCEAN_CHANNEL_SA_SE     0b11100110
	#define ENOCEAN_CHANNEL_SE_SA_SS  0b11110111
	#define ENOCEAN_CHANNEL_SE_SA_CS  0b11110110
	#define ENOCEAN_CHANNEL_SA_CS     0b10110010
	#define ENOCEAN_CHANNEL_SA_SS     0b10110011



	void enocean_init(void);
	void enocean_main(void);
	void enocean_tick(void);
	void enocean_packet_send(uint8_t addr, uint8_t status);

	void enocean_state_set(uint8_t channel, uint8_t state);
	uint8_t enocean_state_get(uint8_t channel);

	extern volatile uint8_t key_state;
	extern volatile uint8_t key_press;
 

#endif // USE_ENOCEAN
#endif // _ENOCEAN_H_



