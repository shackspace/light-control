/*----------------------------------------------------------------------------
------------------------------------------------------------------------------*/
#include "config.h"

#ifndef SHACKBUS_H
#define SHACKBUS_H

	#include <avr/io.h>
	#include <avr/pgmspace.h>
	#include <avr/eeprom.h>
	#include <string.h>
	#include <util/delay.h>
	
	#include "stack.h"
	#include "usart.h"
	#include "cmd.h"

	#include "can.h"


	typedef struct
	{
		uint8_t prio; //Priorität 3bit
		uint8_t vlan; //VLAN 4bit
		uint8_t src;  //SRC 8bit
		uint8_t dst;  //DST 8bit
		uint8_t prot; //prot 6bit
	} shackbus_id_t;

//D018149

	uint8_t shackbus_id2sb(shackbus_id_t *shackbus, can_t msg);
	uint32_t shackbus_sb2id(shackbus_id_t *sb);
 



#endif



