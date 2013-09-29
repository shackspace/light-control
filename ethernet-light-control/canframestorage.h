/*----------------------------------------------------------------------------
------------------------------------------------------------------------------*/
#include "config.h"

#ifndef _CANFRAMESTORAGE_H_
	#define _CANFRAMESTORAGE_H_	


#ifdef USE_CANFRAMESTORAGE

	#include <avr/io.h>
	#include <avr/pgmspace.h>
	#include <avr/eeprom.h>
	#include <string.h>
	#include <util/delay.h>
	
	#include "can2udp.h"

	#define CANFRAMESTORAGE_DATA_SIZE 20

	extern can_frame_t canframestorage_data[];


	void canframestorage_init(void);
	void canframestorage_item_clear(uint8_t unit_id);
    uint8_t canframestorage_item_next(void);
#endif
#endif // _CANFRAMESTORAGE_H_


