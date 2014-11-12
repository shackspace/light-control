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
#include "power_mgt.h"


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




	// Create a test messsage

	send_msg_blink_ret.id = ((3L<<26)+(4L<<22)+(6L<<14)+(5L<<6)+11L);  //Absender = 2   Empfänger = 1
	send_msg_blink_ret.flags.rtr = 0;

	send_msg_blink_ret.flags.extended = 1;

	send_msg_blink_ret.length  = 3;
	send_msg_blink_ret.data[0] = 0;
	send_msg_blink_ret.data[1] = 0;
	send_msg_blink_ret.data[2] = 0;


}

void shackbus_main(void)
{
	// Check if a new messag was received
	if (can_check_message())
	{
		can_t msg;

		// Try to read the message
		if (can_get_message(&msg))
		{
			shackbus_id_t shackbus_id;
			shackbus_id2sb(&shackbus_id,&msg);

			//prot=10 = Basis IO data[0]=10 = Ping
			if (shackbus_id.prot == 10 && shackbus_id.dst == 8 && msg.data[0]==10)
			{
				shackbus_id.prio = 3;
				shackbus_id.vlan = 4;
				shackbus_id.dst  = shackbus_id.src;
				shackbus_id.src  = 8;
				shackbus_id.prot = 10;

				msg.id = shackbus_sb2id(&shackbus_id);
				msg.length = 2;
				msg.data[0] = 11;
				msg.data[1] = msg.data[1];

				//Send the new message
				can_send_message(&msg);
			}

			/* prot=11 = PowerManagement data[0]=1 =on/off data[1]=channel data[2]=state */
			/* channel 140 0x8c = DLE Dusche    */
			/* channel 141 0x8d = DLE E-Lab     */
			/* channel 142 0x8e = Optionsraeume  */
			/* channel 143 0x8f = Kueche         */
			if (shackbus_id.prot == 11 && shackbus_id.dst == 6 && msg.data[0]==1 \
				&& msg.data[1] >= 140 && msg.data[1] <= 143)
			{
				shackbus_id.prio = 3;
				shackbus_id.vlan = 4;
				shackbus_id.dst  = shackbus_id.src;
				shackbus_id.src  = 8;
				shackbus_id.prot = 11;

				msg.id = shackbus_sb2id(&shackbus_id);
				msg.length = 3;
				msg.data[0] = 1;
				msg.data[1] = msg.data[1];
				msg.data[2] = msg.data[2];

				/* Send the message */
				can_send_message(&msg);

				if (msg.data[2]==1) enocean_state_set(msg.data[1],ENOCEAN_CHANNEL_SE_SA_SS);
				if (msg.data[2]==0) enocean_state_set(msg.data[1],ENOCEAN_CHANNEL_SE_SA_CS);
			}



			/* prot=11 = PowerManagement data[0]=0 =on/off data[1]=channel data[2]=state */
			/* channel 140 0x8c = DLE Dusche    */
			/* channel 141 0x8d = DLE E-Lab     */
			/* channel 142 0x8e = Optionsraeume  */
			/* channel 143 0x8f = Kueche         */
			if (shackbus_id.prot == 11 && shackbus_id.dst == 6 && msg.data[0]==0 \
				&& msg.data[1] >= 140 && msg.data[1] <= 143)
			{
				shackbus_id.prio = 3;
				shackbus_id.vlan = 4;
				shackbus_id.dst  = shackbus_id.src;
				shackbus_id.src  = 8;
				shackbus_id.prot = 11;

				msg.id = shackbus_sb2id(&shackbus_id);
				msg.length = 3;
				msg.data[0] = 0;
				msg.data[1] = msg.data[1];
				msg.data[2] = msg.data[2];

				/* Send the message */
				can_send_message(&msg);

				void power_mgt_set_input_1(uint8_t _channel, uint8_t _state);
				void power_mgt_set_wait_off(uint8_t _channel, uint16_t _value);
				power_mgt_set_input_1(msg.data[1]-140, msg.data[2]);
				power_mgt_set_wait_off(msg.data[1]-140, 20);
			}

			/* prot=13 = Hauptschalterstate length=1 data[0]=state */
			/* 0 => OFF    */
			/* 1 => ON     */
			if (shackbus_id.prot == 13 && shackbus_id.dst == 255 && msg.length == 1)
			{
				shackbus_id.prio = 3;
				shackbus_id.vlan = 4;
				shackbus_id.dst  = shackbus_id.src;
				shackbus_id.src  = 8;
				shackbus_id.prot = 13;

				msg.id = shackbus_sb2id(&shackbus_id);
				msg.length = 3;
				msg.data[1] = 2;
				msg.data[2] = 3;

				/* Send the message */
//				can_send_message(&msg);

				if (msg.data[0] > 1)
					msg.data[0] = 0;

				power_mgt_set_input_1 (1, msg.data[0]);
				power_mgt_set_wait_on (1, 4); //DLE Kueche
				power_mgt_set_wait_off(1, 5);
				power_mgt_set_input_1 (2, msg.data[0]);
				power_mgt_set_wait_on (2, 2); //OR
				power_mgt_set_wait_off(2, 4);
				power_mgt_set_input_1 (3, msg.data[0]);
				power_mgt_set_wait_on (3, 3); //Kueche
				power_mgt_set_wait_off(3, 90 * 60); //Sekunden
			}



		}
	}
}

uint8_t shackbus_send_msg(uint8_t val1, uint8_t val2)
{
	//Send the new message
	send_msg_blink_ret.data[0]=1;
	send_msg_blink_ret.data[1]=val1;
	send_msg_blink_ret.data[2]=val2;

	can_send_message(&send_msg_blink_ret);

	return true;
}

void shackbus_tick(void)
{
//	can_t ka;
//	shackbus_id_t ka_id;
//	ka_id.prio = 3;
//	ka_id.vlan = 4;
//	ka_id.dst  = 5;
//	ka_id.src  = 6;
//	ka_id.prot = 10;

//	ka.id = shackbus_sb2id(&ka_id);
//	ka.length = 2;
//	ka.data[0] = 23;
//	ka.data[1] = 42;
				
//	can_send_message(&ka);
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
