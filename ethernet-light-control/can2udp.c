/*----------------------------------------------------------------------------
 Copyright:      Stefan Krupop  mailto: mail@stefankrupop.de
 Author:         Stefan Krupop
 Remarks:        
 known Problems: none
 Version:        17.01.2009
 Description:    Implementation des ArtNet-Protokolls für DMX-Übertragung über Ethernet

 Dieses Programm ist freie Software. Sie können es unter den Bedingungen der 
 GNU General Public License, wie von der Free Software Foundation veröffentlicht, 
 weitergeben und/oder modifizieren, entweder gemäß Version 2 der Lizenz oder 
 (nach Ihrer Option) jeder späteren Version. 

 Die Veröffentlichung dieses Programms erfolgt in der Hoffnung, 
 daß es Ihnen von Nutzen sein wird, aber OHNE IRGENDEINE GARANTIE, 
 sogar ohne die implizite Garantie der MARKTREIFE oder der VERWENDBARKEIT 
 FÜR EINEN BESTIMMTEN ZWECK. Details finden Sie in der GNU General Public License. 

 Sie sollten eine Kopie der GNU General Public License zusammen mit diesem 
 Programm erhalten haben. 
 Falls nicht, schreiben Sie an die Free Software Foundation, 
 Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA. 
------------------------------------------------------------------------------*/
#include "config.h"


#include "can2udp.h"
#include "enocean.h"
#include "shackbus.h"

#ifdef USE_ENOCEAN

#include <stdio.h>
#include <avr/interrupt.h>

#include "networkcard/enc28j60.h"

#include "can.h"

#include "fifo.h"

#include "mem-check.h"

#include "framestorage.h"

// ----------------------------------------------------------------------------
// default values
#define PORT_DEFAULT			4223

// ----------------------------------------------------------------------------
//global variables
unsigned short can2udp_port = PORT_DEFAULT;

uint32_t c2u_remote_ip = IP(0,0,0,0);

uint8_t c2u_counter_tx_dst = 0;  //syn fern>nah
uint8_t c2u_counter_rx_src = 0;  //
uint8_t c2u_counter_rx_dst = 0;  //ack nah>fern

uint8_t c2u_counter_tx_seq = 0;  //syn nah>fern
uint8_t c2u_counter_rx_ack = 0;  //bestätigung von empfangenen bytes


uint8_t c2u_state          = 0;  //vom sender
uint8_t c2u_con_state      = 0;  //0=Standby 1=syn 2=work
uint8_t c2u_con_state_tx   = 0;  //0=Standby 1=syn 2=work 3=work+sendack


uint8_t lost_can_frames = 0;
uint8_t lost_can_frames2 = 0;
uint8_t lost_can_frames3 = 0;




uint8_t fifo_rec_count_read = 0;
uint8_t fifo_rec_count_write = 0;

#define FIFO_REC_BUFFER_SIZE 10

uint8_t fifo_rec_write = 0;
uint8_t fifo_rec_ack   = 0;
uint8_t fifo_rec_read  = 0;

can_frame_t fifo_rec[FIFO_REC_BUFFER_SIZE];


static uint8_t max_can2udp_infifo = 0;

uint8_t time_to_send_ack = 0;

// ----------------------------------------------------------------------------
// initialization of network settings
void can2udp_netInit(void) {


 // remove any existing app from port
 kill_udp_app(can2udp_port);
 // add port to stack with callback
 add_udp_app(can2udp_port, (void(*)(unsigned char))can2udp_get);

// c2u_remote_ip = IP(10,42,3,176);


}



fifo_t can2udp_infifo;
static uint8_t inbuf[10];
fifo_t can2udp_outfifo;
static uint8_t outbuf[10];


// ----------------------------------------------------------------------------
// initialization of Art-Net
void can2udp_init(void) {

  can2udp_port = 4223;

  fifo_init (&can2udp_infifo,   inbuf, 10);
  fifo_init (&can2udp_outfifo,   outbuf, 10);

 return;
}


// ----------------------------------------------------------------------------
// 
void can_frame_t_2_can_t(can_frame_t* a, can_t* b) {

	b->id = a->can_id;	
	b->length = a->can_dlc;
	memcpy(b->data,a->data,a->can_dlc);

	if (b->id & 0x80000000) {
		b->flags.extended = 1;
	} else {
		b->flags.extended = 0;
	}
	b->id &= 0x1FFFFFFF;
}

// ----------------------------------------------------------------------------
// 
void can_t_2_can_frame_t(can_t* a, can_frame_t* b) {

	memset(b,0,sizeof(can_frame_t));    
	b->can_id = a->id;	
	b->can_dlc = a->length;
	memcpy(b->data,a->data,a->length);

	if(a->flags.extended==1)
		b->can_id |= 0x80000000;
}




void can2udp_ethernet(uint8_t mode);

void can2udp(can_t *msg)
{

	uint8_t nextfreeid = framestorage_item_next();
	if (can2udp_fifo_fs_id(nextfreeid))
	{
		memcpy(&framestorage_data[nextfreeid], msg, sizeof(FS_DATA_TYPE));
		return;
	} else {
		if (fifo_get_count(&can2udp_outfifo) > 8) lost_can_frames++;
		if (nextfreeid == 255)                 lost_can_frames2++;
	}
}

uint8_t can2udp_fifo_fs_id(uint8_t id)
{
	if(fifo_get_count(&can2udp_outfifo) > 8)
		return false;
	if(id==255)
		return false;
	if(can2udp_outfifo.count >= can2udp_outfifo.size)
		return false;
	if(framestorage_put(id)==0)
		return false;
	if(fifo_put (&can2udp_outfifo,id)==false)
		return false;

	return true;
}


uint8_t can2udp_get_next_message(uint8_t* msg);

uint8_t can2udp_get_next_message(uint8_t* msg)
{


//	*((uint32_t*)&eth_buffer[UDP_DATA_START+0]) = *((uint32_t*)&msg->id);
//	if(msg->flags.extended==1)
//		eth_buffer[UDP_DATA_START+3] |= 0x80;
//	eth_buffer[UDP_DATA_START+4] = msg->length; //data_length_code
//	memcpy(&eth_buffer[UDP_DATA_START+8],(uint8_t*)&msg->data[0],msg->length);

	if (fifo_get_count(&can2udp_outfifo) > 0)
	{
		uint8_t cur_nr = fifo_get (&can2udp_outfifo);
		can_t_2_can_frame_t(&framestorage_data[cur_nr], msg);
		framestorage_get(cur_nr);

		return 1;
	}


	return 0;
}

void can2udp_ethernet(uint8_t mode)
{
	memset(&eth_buffer[UDP_DATA_START+0],0,sizeof(can_frame_t));

	uint8_t retval = 0;

	retval = can2udp_get_next_message(&eth_buffer[UDP_DATA_START+0]);

	// Wenn Verbindung nicht in betrieb, dann auch nicht versuchen was zu senden.
	if (c2u_con_state != 2)
		retval = 0;
	


	if (c2u_con_state_tx == 1)
	{
		c2u_con_state_tx = 2; //work
		eth_buffer[UDP_DATA_START+5] = 6; //Ack+syn
	}
	else if (c2u_con_state_tx == 3) // normal work + send ack
	{
		c2u_con_state_tx = 2; //work
		eth_buffer[UDP_DATA_START+5] = 2; //ACK
		time_to_send_ack = 10; //100ms
	} else {
		if (retval==0) return;	//Es ist kein Ack zu senden und keine Daten vorhanden
	}

	if (retval)
		eth_buffer[UDP_DATA_START+5] |= 1;

	if (retval)
		c2u_counter_tx_seq++;
	eth_buffer[UDP_DATA_START+6] = c2u_counter_tx_seq;


//	eth_buffer[UDP_DATA_START+8] = mode;
//	eth_buffer[UDP_DATA_START+9] = retval;
//	eth_buffer[UDP_DATA_START+10]= eth_buffer[UDP_DATA_START+4];
//	eth_buffer[UDP_DATA_START+4] = 3;
//	static uint8_t ctr_cnt = 0;
//	eth_buffer[UDP_DATA_START+15] = ctr_cnt++;



	eth_buffer[UDP_DATA_START+7] = c2u_counter_rx_ack+1;


	if ((arp_entry_search(c2u_remote_ip) != MAX_ARP_ENTRY) && (c2u_remote_ip))
		create_new_udp_packet( sizeof(can_frame_t), 4223, 4223, c2u_remote_ip );


}



uint8_t can2udp_timer_flag = 0;
uint8_t can2udp_timer_flag_10ms = 0;


void can2udp_main(void) {


	if ( max_can2udp_infifo < fifo_get_count(&can2udp_infifo) )
		max_can2udp_infifo = fifo_get_count(&can2udp_infifo);



	if ( (fifo_get_count(&can2udp_infifo) > 0) && can_check_free_buffer() ) {

		uint8_t cur_nr = fifo_get (&can2udp_infifo);

		if ( can_send_message_fifo_fs_id(cur_nr) != 0 )
			lost_can_frames++;

		framestorage_get(cur_nr);
		//can_check_free_buffer();
		//can2udp(&udp_snd_can_frame);

	}


	{
		can2udp_ethernet(1);
	}


	if (c2u_con_state_tx == 2 && time_to_send_ack == 0)
		c2u_con_state_tx = 3; //Sende ein Ack.


  if (can2udp_timer_flag)
  {
	can2udp_timer_flag = 0;
	static uint8_t c2u_arp_in_request = 0;
	if (c2u_arp_in_request) c2u_arp_in_request--;
	uint8_t c2u_entr_i = arp_entry_search(c2u_remote_ip);
	if (*(uint32_t*)myip && c2u_remote_ip)
	{
		if ( (c2u_entr_i == MAX_ARP_ENTRY || arp_entry[c2u_entr_i].arp_t_time < 10) && c2u_arp_in_request == 0 )
		{
			c2u_arp_in_request = 60;
			arp_request(c2u_remote_ip);
		}
	}



	static uint8_t counter = 3;
	
	if (!counter--)
	{

		counter=3;
		counter=5;

		static can_t msg;
		msg.id = 0x12345678;
		msg.flags.extended = 1;
		msg.length = 8;
		uint16_t tmp_cnt = get_mem_unused();		
		msg.data[0]=(tmp_cnt>>8)&0xFF;
		msg.data[1]=tmp_cnt&0xFF;
		msg.data[2]=sizeof(can_t);
		msg.data[3]=max_can2udp_infifo;
		msg.data[4]=lost_can_frames;
		msg.data[5]=lost_can_frames2;
		msg.data[6]=lost_can_frames3;
		msg.data[7]=0xEF;

		static shackbus_id_t sb;
		sb.prio = 255;
		sb.vlan = 255;
		sb.src = 255;
		sb.dst = 255;
		sb.prot = 255;

		msg.id = shackbus_sb2id(&sb);

		//fifo_put_var (&can2udp_infifo,(uint8_t *)&msg,sizeof(can_t));
		can2udp(&msg);


	}



  }



}


// ----------------------------------------------------------------------------
// receive CAN packet
void can2udp_get(unsigned char index) {

	if ( (UDP_DATA_END_VAR - UDP_DATA_START) != 16)
		return;

	
	//c2u_counter_rx_src = 0;
	c2u_state          = eth_buffer[UDP_DATA_START+5];
	c2u_counter_rx_dst = eth_buffer[UDP_DATA_START+6];
	c2u_counter_tx_dst = eth_buffer[UDP_DATA_START+7];

	if ( c2u_state & _BV(4) ) //Reset
	{
		c2u_counter_tx_seq = 0;
		c2u_counter_rx_src = 0;
		c2u_counter_tx_dst = 0;
		c2u_counter_rx_dst = 0;
		c2u_counter_rx_ack = 0;

		c2u_con_state = 0;
		c2u_con_state_tx = 0;
	
		c2u_remote_ip = 0;
	}

	
	if ( c2u_con_state == 0)
	{
		if ( c2u_state & _BV(2) ) //Get SYN
		{
			c2u_counter_rx_src = c2u_counter_rx_dst;

			c2u_con_state_tx = 1; //erzwingt das versenden von ACK+SYN
			c2u_con_state = 1;

			//IP-Adresse als neues Ziel übernehmen.
			struct IP_Header *ip;
			ip = (struct IP_Header *)&eth_buffer[IP_OFFSET];
			c2u_remote_ip = ip->IP_Srcaddr;

			can2udp_ethernet(0); 	
		}
	}
	else if (c2u_con_state == 1)
	{
		if ( c2u_state & _BV(1) ) //Get Ack
		{
			c2u_counter_tx_seq = c2u_counter_tx_dst - 1;

			c2u_con_state = 2;
		}
	}
	else if (c2u_con_state == 2)
	{
		if ( c2u_state & _BV(0) ) //Data available
		{
			static can_frame_t udp_rec_can_frame;
			memset(&udp_rec_can_frame,0,sizeof(can_frame_t));

			if ((c2u_counter_rx_src + 1)%256 != c2u_counter_rx_dst)
			{
				uint8_t err_msg[] = {2,0,0,0xA0,0,0,0,0,0,0,0,0,0,0};			
				err_msg[5] = 2;
				err_msg[6] = (c2u_counter_rx_src + 1)%256;
				err_msg[7] = c2u_counter_rx_dst;
				can2udp((can_t*)&err_msg);
				return;
			} else {
				c2u_con_state_tx = 3;
				c2u_counter_rx_ack = c2u_counter_rx_dst;
				c2u_counter_rx_src = c2u_counter_rx_dst;
			}

	
			*((uint32_t*) &udp_rec_can_frame.can_id) = *((uint32_t*)&eth_buffer[UDP_DATA_START+0]); 
			udp_rec_can_frame.can_dlc = eth_buffer[UDP_DATA_START+4];
			memcpy(udp_rec_can_frame.data,(uint8_t*)&eth_buffer[UDP_DATA_START+8],udp_rec_can_frame.can_dlc);


			static can_t       udp_snd_can_frame;
			can_frame_t_2_can_t(&udp_rec_can_frame, &udp_snd_can_frame);
		//	can2udp(&udp_snd_can_frame); /* TODO */
		//	can_send_message(&udp_snd_can_frame); //direkt senden an mcp2515

		//	change_light_state(udp_snd_can_frame.data[0],udp_snd_can_frame.data[1]);

			uint8_t nextfreeid = framestorage_item_next();
			if (fifo_get_count(&can2udp_infifo) <= 8 && nextfreeid != 255 )
			{
				can_frame_t_2_can_t(&udp_rec_can_frame, &framestorage_data[nextfreeid]);
				fifo_put (&can2udp_infifo,nextfreeid);
				framestorage_put(nextfreeid);
			} else
				lost_can_frames3++;

		}
	}
}


// ----------------------------------------------------------------------------
// Called by timer, check changes
void can2udp_tick(void) {

	can2udp_timer_flag = 1;

}

// ----------------------------------------------------------------------------
// Called by timer, check changes
void can2udp_tick_10ms(void) {

	if (time_to_send_ack)
	{
		time_to_send_ack--;
	}
}





#endif
