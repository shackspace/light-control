/*----------------------------------------------------------------------------
 Copyright:      Ulrich Escher  mailto: code@myvdr.de
 Author:         Ulrich Escher
 Remarks:        
 known Problems: none
 Version:        11.04.2012
 Description:    Header-Datei für das allgemeine CAN Interface für Shackbus

------------------------------------------------------------------------------*/

#ifndef SHACKBUS_H
#define SHACKBUS_H

#include "config.h"
#if USE_SHACKBUS

	#include "can.h"

	void shackbus_init(void);
	void shackbus_main(void);
	void shackbus_tick(void);

	uint8_t shackbus_send_msg(uint8_t val1, uint8_t val2);
	uint8_t can_send_message_fifo(const can_t *msg);
	uint8_t can_send_message_fifo_fs_id(uint8_t id);

	typedef struct
	{
		uint8_t prio; //Priorität 3bit
		uint8_t vlan; //VLAN 4bit
		uint8_t src;  //SRC 8bit
		uint8_t dst;  //DST 8bit
		uint8_t prot; //prot 6bit
	} shackbus_id_t;

	uint8_t shackbus_id2sb(shackbus_id_t *shackbus, can_t *msg);
	uint32_t shackbus_sb2id(shackbus_id_t *sb);

#endif // USE_SHACKBUS
#endif // CAN_H

