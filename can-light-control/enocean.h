/*----------------------------------------------------------------------------
 Copyright:      Ulrich Escher  mailto: code@myvdr.de
 Author:         Ulrich Escher
 Remarks:        
 known Problems: none
 Version:        13.01.2012
 Description:    enocean

 implement enocean

------------------------------------------------------------------------------*/

#include "config.h"

#ifdef USE_ENOCEAN

	#include <avr/io.h>
	#include <avr/pgmspace.h>
	#include <avr/eeprom.h>
	#include <string.h>
	
	#include "uart.h"

	#define ENOCEAN_LICHT_EEPROM_STORE    	60

	#define ENOCEAN_CHANNEL_STATUS  1
	#define ENOCEAN_CHANNEL_ACT     2
	#define ENOCEAN_CHANNEL_EEPROM  4

	void enocean_init(void);
	void enocean_main(void);
	void enocean_tick(void);
	void enocean_packet_send(uint8_t addr, uint8_t status);

	void enocean_state_set(uint8_t channel, uint8_t state);

	extern volatile uint8_t key_state;
	extern volatile uint8_t key_press;
 

#endif



