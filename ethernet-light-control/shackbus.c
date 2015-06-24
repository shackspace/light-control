/*----------------------------------------------------------------------------
 Copyright:      Ulrich Escher  mailto: code@myvdr.de
 Author:         Ulrich Escher
 Remarks:        
 known Problems: none
 Version:        11.04.2012
 Description:    Allgemeines shackbus Interface

------------------------------------------------------------------------------*/

#include "config.h"
#if USE_SHACKBUS

#include <stdint.h>
#include <avr/io.h>

#include "shackbus.h"
#include "can.h"
#include "enocean.h"
#include "can2udp.h"
#include "fifo.h"
#include "framestorage.h"

fifo_t can_outfifo;
static uint8_t can_outbuf[10];

const uint8_t PROGMEM can_filter[] = 
{
	// Group 0
	MCP2515_FILTER(0),				// Filter 0
	MCP2515_FILTER(0),				// Filter 1

	// Group 1
	MCP2515_FILTER_EXTENDED(0),		// Filter 2
	MCP2515_FILTER_EXTENDED(0),		// Filter 3
	MCP2515_FILTER_EXTENDED(0),		// Filter 4
	MCP2515_FILTER_EXTENDED(0),		// Filter 5

	MCP2515_FILTER(0),				// Mask 0 (for group 0)
	MCP2515_FILTER_EXTENDED(0),		// Mask 1 (for group 1)
};
// You can receive 11 bit identifiers with either group 0 or 1.


void shackbus_init(void)
{
	// Initialize MCP2515
	can_init(BITRATE_125_KBPS);
#ifdef UART_DEBUG
	uart_write("can_init(BITRATE_125_KBPS);");
#endif

	// Load filters and masks
	can_static_filter(can_filter);
#ifdef UART_DEBUG
	uart_write("can_static_filter(can_filter);");
#endif

	fifo_init (&can_outfifo,   can_outbuf, 10);
	framestorage_init();


}

void shackbus_main(void)
{
	if ( fifo_get_count(&can_outfifo) > 0  )
	{
		uint8_t cur_nr = fifo_read (&can_outfifo);
		if ( can_send_message(&framestorage_data[cur_nr]) )
		{
			fifo_get (&can_outfifo);
			framestorage_get(cur_nr);
		}
	}

	// Check if a new messag was received
	if (can_check_message())
	{
		can_t msg;

		// Try to read the message
		if (can_get_message(&msg))
		{
			shackbus_id_t shackbus_id;
			shackbus_id2sb(&shackbus_id,&msg);

			can2udp(&msg); //msg an udp weiterleiten

			/* prot=9 = Enocean data[0]=channel data[1]=state */
			if (shackbus_id.prot == 9 && msg.data[0]<12 )
			{
				send_udp_msg(msg.data[0], msg.data[1]);
			}

			/* prot=9 = Enocean data[0]=channel data[1]=state */
			/* 120 = main switch */
			if (shackbus_id.prot == 9 && msg.data[0]==120 )
			{
				send_udp_msg(msg.data[0], msg.data[1]);
			}

			/* prot=11 = PowerManagement data[0]=0 =on/off data[1]=channel data[2]=state */
			if (shackbus_id.prot == 11 && msg.data[0]==1 )
			{
				send_udp_msg(msg.data[1], msg.data[2]);
			}
		}
	}
}


uint8_t can_send_message_fifo(const can_t *msg)
{
	uint8_t nextfreeid = framestorage_item_next();
	if(can_send_message_fifo_fs_id(nextfreeid))
	{
		memcpy(&framestorage_data[nextfreeid], msg, sizeof(FS_DATA_TYPE));
	} else {
		return false;
	}
	return true;
}

uint8_t can_send_message_fifo_fs_id(uint8_t id)
{
	if(fifo_get_count(&can_outfifo) > 8)
		return false;
	if(id==255)
		return false;
	if(can_outfifo.count >= can_outfifo.size)
		return false;
	if(framestorage_put(id)==0)
		return false;
	if(fifo_put (&can_outfifo,id)==false)
		return false;

	return true;
}

void shackbus_tick(void)
{

}

uint8_t shackbus_id2sb(shackbus_id_t* shackbus, can_t* msg)
{
	shackbus->prio = (msg->id >> 26) & 0x07;
	shackbus->vlan = (msg->id >> 22) & 0x0F;
	shackbus->src  = (msg->id >> 14) & 0xFF;
	shackbus->dst  = (msg->id >> 06) & 0xFF;
	shackbus->prot = (msg->id >> 00) & 0x3F;

	return 0;
}

uint32_t shackbus_sb2id(shackbus_id_t* sb)
{
	sb->prio &= 0x07;
	sb->vlan &= 0x0F;
	sb->src  &= 0xFF;
	sb->dst  &= 0xFF;
	sb->prot &= 0x3F;

	return (uint32_t)((uint32_t)0x80000000+((uint32_t)sb->prio<<26)+((uint32_t)sb->vlan<<22)+((uint32_t)sb->src<<14)+((uint32_t)sb->dst<<6)+(uint32_t)sb->prot);
}

#endif // USE_SHACKBUS
