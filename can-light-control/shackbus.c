/*----------------------------------------------------------------------------
 Copyright:      Ulrich Escher  mailto: code@myvdr.de
 Author:         Ulrich Escher
 Remarks:        
 known Problems: none
 Version:        11.04.2012
 Description:    shackbus Interface


------------------------------------------------------------------------------*/

#include "config.h"
#if USE_SHACKBUS

#include <avr/io.h>
#include "shackbus.h"

#include "can.h"



uint8_t can_filter[] = 
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


    can_t send_msg;
	can_t send_msg_blink_ret;
    can_t send_test_msg;
	can_t send_msg_blink;


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
	
	send_msg.id = 0x00010202;
	send_msg.flags.rtr = 0;
	send_msg.flags.extended = 1;
	
	send_msg.length = 2;
	send_msg.data[0] = 0x00;
	send_msg.data[1] = 0x00;



	// Create a test messsage
	send_msg_blink_ret.id = ((3L<<26)+(4L<<22)+(6L<<14)+(5L<<6)+9L);  //Absender = 2   Empfänger = 1
	send_msg_blink_ret.flags.rtr = 0;

	send_msg_blink_ret.flags.extended = 1;

	send_msg_blink_ret.length  = 3;
	send_msg_blink_ret.data[0] = 3;
	send_msg_blink_ret.data[1] = 1;
	send_msg_blink_ret.data[2] = 0;



    // Create a test messsage
	
	send_test_msg.id = 0x00010203;
	send_test_msg.flags.rtr = 0;
	send_test_msg.flags.extended = 1;
	
	send_test_msg.length = 8;
	send_test_msg.data[0] = 0x00;
	send_test_msg.data[1] = 0x00;


	// Create a test messsage

	send_msg_blink.id = 0x00020109;  //Absender = 2   Empfänger = 1
	send_msg_blink.flags.rtr = 0;

	send_msg_blink.flags.extended = 1;

	send_msg_blink.length  = 3;
	send_msg_blink.data[0] = 3;
	send_msg_blink.data[1] = 1;
	send_msg_blink.data[2] = 0;





}


//uint8_t shackbus_convert_id(shackbus_id_t* shackbus, can_t msg)

uint8_t shackbus_id2sb(shackbus_id_t* shackbus, can_t msg)
{

  shackbus->prio = (msg.id >> 26) & 0x07;
  shackbus->vlan = (msg.id >> 22) & 0x0F;
  shackbus->src  = (msg.id >> 14) & 0xFF;
  shackbus->dst  = (msg.id >> 06) & 0xFF;
  shackbus->prot = (msg.id >> 00) & 0x3F;

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




void shackbus_main(void)
{
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
              shackbus_id2sb(&shackbus_id,msg);

			  //	send_msg_blink.id = (((((((3<<3)+4L) <<4)+5L) <<8)+6L) <<8)+9;  //Absender = 2   Empfänger = 1

              if (shackbus_id.prot == 9 && shackbus_id.dst == 6 && msg.data[0]<12)
			  {
                if (msg.data[1]==1) enocean_state_set(msg.data[0],ENOCEAN_CHANNEL_SE_SA_SS);
                if (msg.data[1]==0) enocean_state_set(msg.data[0],ENOCEAN_CHANNEL_SE_SA_CS);

			    // Send the new message
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
				

			    // Send the new message
				can_send_message(&msg);

				void reset_visualisation(void);
				reset_visualisation();
			  }



            }
		}

}

uint8_t shackbus_send_msg(uint8_t val1, uint8_t val2)
{
    // Send the new message
    send_msg_blink_ret.data[0]=val1;
    send_msg_blink_ret.data[1]=val2;
    send_msg_blink_ret.data[2]++;
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


#endif // USE_SHACKBUS
