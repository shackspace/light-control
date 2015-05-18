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
#include <avr/wdt.h>

#include "timer.h"
#include "shackbus.h"
#include "can.h"
#include "enocean.h"
#include "hmi.h"
#include "fifo.h"
#include "framestorage.h"


uint8_t shackbus_msg_send_flag = 0;

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

can_t send_msg_blink_ret;

uint8_t shackbus_system_time_send_flag = 0;

void shackbus_startup_message(void);

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



	// Create a test messsage
	send_msg_blink_ret.id = ((3L<<26)+(4L<<22)+(6L<<14)+(5L<<6)+9L);
	send_msg_blink_ret.flags.rtr = 0;

	send_msg_blink_ret.flags.extended = 1;

	send_msg_blink_ret.length  = 3;
	send_msg_blink_ret.data[0] = 3;
	send_msg_blink_ret.data[1] = 1;
	send_msg_blink_ret.data[2] = 0;

	shackbus_startup_message();

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

	if (shackbus_msg_send_flag)
	{
		can_t msg;
		shackbus_id_t shackbus_id;


		shackbus_msg_send_flag = 0;

		shackbus_id.prio = 3;
		shackbus_id.vlan = 4;
		shackbus_id.dst  = 255;
		shackbus_id.src  = 6;
		shackbus_id.prot = 13;

		memset(&msg,0,sizeof(can_t));
		msg.id = shackbus_sb2id(&shackbus_id);
		msg.length = 1;
		msg.flags.extended = 1;
		msg.flags.rtr = 0;

		if (MAINSWITCH_STATE)
			msg.data[0] = 1;
		else
			msg.data[0] = 0;

		//Send the new message
		can_send_message_fifo(&msg);
	}


	// Check if a new messag was received
	if (can_check_message())
	{
		can_t msg;

		// Try to read the message
		if (can_get_message(&msg))
		{
			if (msg.id == 0x1FF && msg.flags.extended == 1)
			{
				led_set(99,0);
			}
			shackbus_id_t shackbus_id;
			shackbus_id2sb(&shackbus_id,&msg);

			if (shackbus_id.prot == 9 && shackbus_id.dst == 6 && msg.data[0]<12)
			{
				if (msg.data[1]==1) enocean_state_set(msg.data[0],ENOCEAN_CHANNEL_SE_SA_SS);
				if (msg.data[1]==0) enocean_state_set(msg.data[0],ENOCEAN_CHANNEL_SE_SA_CS);

				//Send the new message
				//shackbus_send_msg(msg.data[0], msg.data[1]);
			}

			//prot=10 = Basis IO data[0]=10 = Ping
			if (shackbus_id.prot == 10 && shackbus_id.dst == 6 && msg.data[0]==10)
			{
				shackbus_id.prio = 3;
				shackbus_id.vlan = 4;
				shackbus_id.dst  = shackbus_id.src;
				shackbus_id.src  = 6;
				shackbus_id.prot = 10;

				msg.id = shackbus_sb2id(&shackbus_id);
				msg.length = 2;
				msg.data[0] = 11;
				msg.data[1] = msg.data[1];

				//Send the new message
				can_send_message_fifo(&msg);
			}

			//prot=10 = Basis IO data[0]=04 = Jump2Bootloader
			if (shackbus_id.prot == 10 && shackbus_id.dst == 6 && msg.data[0]==04)
			{
				cli();
				wdt_enable(WDTO_15MS);
				while (1);
			}

			/* prot=11 = PowerManagement data[0]=1 =on/off data[1]=channel data[2]=state */
			/* channel 120 = Hauptschalter      */
			/* channel 140 0x8c = DLE Dusche    */
			/* channel 141 0x8d = DLE E-Lab     */
			/* channel 142 0x8e = Optionsraeume  */
			/* channel 143 0x8f = Kueche         */
			if (shackbus_id.prot == 11 && shackbus_id.dst == 6 && msg.data[0]==1)
			{
				shackbus_id.prio = 3;
				shackbus_id.vlan = 4;
				shackbus_id.dst  = shackbus_id.src;
				shackbus_id.src  = 6;
				shackbus_id.prot = 11;

				msg.id = shackbus_sb2id(&shackbus_id);
				msg.length = 3;
				msg.data[0] = 1;
				msg.data[1] = msg.data[1];
				msg.data[2] = msg.data[2];

				/* Send the message */
				can_send_message_fifo(&msg);
				enocean_packet_send(msg.data[1], msg.data[2]);
			}
		}
	}

	if (shackbus_system_time_send_flag >= 60)
	{
		shackbus_system_time_send_flag = 0;

		shackbus_id_t ka_id;
		ka_id.prio = 1;
		ka_id.vlan = 3;
		ka_id.dst  = 255;
		ka_id.src  = 6;
		ka_id.prot = 2;

		can_t ka;
		memset(&ka,0,sizeof(can_t));
		ka.id = shackbus_sb2id(&ka_id);
		ka.length = 8;

		ka.flags.extended = 1;
		ka.flags.rtr = 0;

		uint8_t sreg = SREG;
		cli();

		for (uint8_t i = 0;i<8;i++)
			ka.data[i] = system_time.type8[i];

		SREG = sreg;

		can_send_message_fifo(&ka);
	}

}

uint8_t shackbus_send_msg(uint8_t val1, uint8_t val2)
{
	//Send the new message
	send_msg_blink_ret.data[0]=val1;
	send_msg_blink_ret.data[1]=val2;
	send_msg_blink_ret.data[2]++;
	return can_send_message_fifo(&send_msg_blink_ret);
}

uint8_t can_send_message_fifo(const can_t *msg)
{
	uint8_t nextfreeid = framestorage_item_next();
	if(fifo_get_count(&can_outfifo) <= 8 && nextfreeid != 255)
	{
		memcpy(&framestorage_data[nextfreeid], msg, sizeof(FS_DATA_TYPE));
		framestorage_put(nextfreeid);
		fifo_put (&can_outfifo,nextfreeid);
	} else {
		return false;
	}
	return true;
}

void shackbus_tick(void)
{
	shackbus_msg_send_flag = 1;

	shackbus_system_time_send_flag++;

	return;
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

void shackbus_startup_message(void)
{
		shackbus_id_t ka_id;
		ka_id.prio = 1;
		ka_id.vlan = 3;
		ka_id.dst  = 255;
		ka_id.src  = 6;
		ka_id.prot = 1;

		can_t ka;
		memset(&ka,0,sizeof(can_t));
		ka.id = shackbus_sb2id(&ka_id);
		ka.length = 8;

		ka.flags.extended = 1;
		ka.flags.rtr = 0;

		ka.data[0] = 0;
		ka.data[1] = 0;
		ka.data[2] = 0;
		ka.data[3] = 0;
		ka.data[4] = 0;
		ka.data[5] = 0;
		ka.data[6] = 0;
		ka.data[7] = 0;

		can_send_message_fifo(&ka);
}

#endif // USE_SHACKBUS
