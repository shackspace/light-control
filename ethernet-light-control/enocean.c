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
#include "enocean.h"

#ifdef USE_ENOCEAN

#include <stdio.h>
#include <avr/interrupt.h>

#include "networkcard/enc28j60.h"

#include "can.h"

#include "shackbus.h"

#include "can2udp.h"

// ----------------------------------------------------------------------------
// default values
#define PORT_DEFAULT			1337

// ----------------------------------------------------------------------------
//global variables
unsigned short enocean_port = PORT_DEFAULT;



uint8_t licht_status[12];
#define ENOCEAN_CHANNEL_STATUS	1
#define ENOCEAN_CHANNEL_ACT		2

uint8_t power_status[4];

uint8_t blinker;
uint8_t blinker_cnt;

uint32_t remote_ip = IP(0,0,0,0);
uint32_t remote_ip_openhab = IP(0,0,0,0);

//internal function prototyps
void change_light_state(uint8_t addr, uint8_t state);


// ----------------------------------------------------------------------------
// initialization of network settings
void enocean_netInit(void)
{
	// calculate broadcast adress
	*((uint32_t*)broadcast_ip) = (((*((uint32_t*)myip)) & (*((uint32_t*)&netmask[0]))) | (~(*((uint32_t*)&netmask[0]))));

	// remove any existing app from port
	kill_udp_app(enocean_port);
	// add port to stack with callback
	add_udp_app(enocean_port, (void(*)(unsigned char))enocean_get);

	// remote_ip = IP(10,42,3,176);
	remote_ip = IP(10,42,0,111);
	// remote_ip = IP(192,168,2,100);
	// remote_ip = IP(192,168,2,100);

	remote_ip_openhab = IP(10,42,0,117);
}


// ----------------------------------------------------------------------------
// initialization of enocean
void enocean_init(void)
{
	// read Art-Net port
	eeprom_read_block(&enocean_port, (unsigned char *)ENOCEAN_PORT_EEPROM_STORE, 2);
	if (enocean_port == 0xFFFF) {
		enocean_port = PORT_DEFAULT;
	}

	DDRD  |= 0xF8;	//PD4-PD7 = LEDs  PD3 = Hauptschalter LED
	PORTD &= 0x07;

	DDRC  &= 0xE0;	//PC0-3 = Tastereingänge  PC4 = Hauptschalter
	PORTC |= 0x1F;

	for (uint8_t i = 0; i < 12; i++)
	{
		licht_status[i] = 0;

		if (licht_status[i] == 0xFF)
		{
			licht_status[i] = 0;
		}
	}

	for (uint8_t i = 0; i < 4; i++)
	{
		power_status[i] = 0;
	}

	return;
}


void send_udp_msg(uint8_t addr, uint8_t cmd)
{
	if (addr >= 0 && addr < 12)
	{
		eth_buffer[UDP_DATA_START+0]=addr+20;
		eth_buffer[UDP_DATA_START+1]=cmd;

		create_new_udp_packet(2, 2342, 2342, remote_ip);
		create_new_udp_packet(2, 2342, 2342, remote_ip_openhab);
	}

	if (addr >= 0 && addr <= 10)
	{
		if (addr == 0) addr = 0;
		else if (addr == 1) addr = 0;
		else if (addr == 2) addr = 1;
		else if (addr == 3) addr = 1;
		else if (addr == 4) addr = 2;
		else if (addr == 5) addr = 3;
		else if (addr == 6) addr = 4;
		else if (addr == 7) addr = 4;
		else if (addr == 8) addr = 5;
		else if (addr == 9) addr = 6;
		else if (addr == 10) addr = 7;

		eth_buffer[UDP_DATA_START+0]=addr;
		eth_buffer[UDP_DATA_START+1]=cmd;

		create_new_udp_packet(2, 2342, 2342, 0xffffffff);
		create_new_udp_packet(2, 2341, 2341, 0xffffffff); //zweiter Port damit auf einem Rechner zwei Programme die events abfangen koennen
	}	

	if (addr == 120 || ( addr >= 140 && addr <= 143))
	{
		if (addr == 120) addr = 10;

		eth_buffer[UDP_DATA_START+0]=addr;
		eth_buffer[UDP_DATA_START+1]=cmd;

		create_new_udp_packet(2, 2342, 2342, 0xffffffff);
		create_new_udp_packet(2, 2342, 2342, remote_ip);
		create_new_udp_packet(2, 2342, 2342, remote_ip_openhab);

	}	

	return;
}


void enocean_main(void)
{
	if (blinker)
	{
		blinker = 0;		
		can_error_register_t aktuelle_fehler = can_read_error_register();

		can_t msg;
		msg.flags.extended = 1;
		msg.length = 2;
		msg.data[0]=aktuelle_fehler.rx;
		msg.data[1]=aktuelle_fehler.tx;

		shackbus_id_t sb;
		sb.prio = 255;
		sb.vlan = 255;
		sb.src = 255;
		sb.dst = 255;
		sb.prot = 255;

		msg.id = shackbus_sb2id(&sb);

//		can2udp(&msg);

	}

	for (uint8_t i = 0; i < 12; i++)
	{
		if (licht_status[i] & ENOCEAN_CHANNEL_ACT)
		{
			licht_status[i] &= ~ENOCEAN_CHANNEL_ACT;
			enocean_packet_send(i,licht_status[i] & ENOCEAN_CHANNEL_STATUS);	
		}
	}

	for (uint8_t i = 0; i < 4; i++)
	{
		if (power_status[i] & ENOCEAN_CHANNEL_ACT)
		{
			power_status[i] &= ~ENOCEAN_CHANNEL_ACT;
			power_packet_send(i+140,power_status[i] & ENOCEAN_CHANNEL_STATUS);	
		}
	
	}
}


void change_light_state(uint8_t addr, uint8_t state)
{
	licht_status[addr] |= ENOCEAN_CHANNEL_ACT;
	if (state==0)
		licht_status[addr] &= ~ENOCEAN_CHANNEL_STATUS;
	else	
		licht_status[addr] |= ENOCEAN_CHANNEL_STATUS;
}


// ----------------------------------------------------------------------------
// receive Art-Net packet
void enocean_get(unsigned char index)
{
	if(eth_buffer[UDP_DATA_START] == 0xA5 && eth_buffer[UDP_DATA_START+1] == 0x5A)
	{
		uint8_t addr   = eth_buffer[UDP_DATA_START+2];
		uint8_t status = eth_buffer[UDP_DATA_START+3];

		if ((addr) < 12 && status <= 1)
		{
			if (addr == 0) change_light_state(0,status);
			if (addr == 0) change_light_state(1,status);
			if (addr == 1) change_light_state(2,status);
			if (addr == 1) change_light_state(3,status);
			if (addr == 2) change_light_state(4,status);
			if (addr == 3) change_light_state(5,status);
			if (addr == 4) change_light_state(6,status);
			if (addr == 4) change_light_state(7,status);
			if (addr == 5) change_light_state(8,status);
			if (addr == 6) change_light_state(9,status);
			if (addr == 7) change_light_state(10,status);
		}

		if ((addr) >= 20 && (addr) <= 30 && status <= 1)
		{
			addr = addr - 20;			
			change_light_state(addr,status);
		}

		if ((addr) >= 140 && (addr) <= 143 && status <= 1)
		{
			addr = addr - 140;			
			power_status[addr] |= ENOCEAN_CHANNEL_ACT;
			if (status==0)
				power_status[addr] &= ~ENOCEAN_CHANNEL_STATUS;
			else	
				power_status[addr] |= ENOCEAN_CHANNEL_STATUS;
		}
	}
}


// ----------------------------------------------------------------------------
// Called by timer, check changes
void enocean_tick(void)
{
	static uint8_t counter = 1;

	if (!counter--)
	{
		blinker ^= 1;
		counter=0;
		blinker_cnt++;
	}

	static uint8_t arp_in_request = 0;
	if (arp_in_request) arp_in_request--;
	uint8_t entr_i = arp_entry_search(remote_ip);
	if (*(uint32_t*)myip && remote_ip)
	{
		if ( (entr_i == MAX_ARP_ENTRY || arp_entry[entr_i].arp_t_time < 10) && arp_in_request == 0 )
		{
			arp_in_request = 60;
			arp_request(remote_ip);
		}
	}

	static uint8_t arp_in_request_openhab = 5; 
	if (arp_in_request_openhab) arp_in_request_openhab--;
	entr_i = arp_entry_search(remote_ip_openhab);
	if (*(uint32_t*)myip && remote_ip_openhab)
	{
		if ( (entr_i == MAX_ARP_ENTRY || arp_entry[entr_i].arp_t_time < 10) && arp_in_request_openhab == 0 )
		{
			arp_in_request_openhab = 60;
			arp_request(remote_ip_openhab);
		}
	}


can_t msg;
msg.id = 0x12345678;
msg.flags.extended = 1;
msg.length = 8;
msg.data[0]=0x01;
msg.data[1]=0x23;
msg.data[2]=0x45;
msg.data[3]=0x67;
msg.data[4]=0x89;
msg.data[5]=0xAB;
msg.data[6]=0xCD;
msg.data[7]=0xEF;


//can2udp(&msg);

shackbus_id_t sb;
sb.prio = 255;
sb.vlan = 255;
sb.src = 255;
sb.dst = 255;
sb.prot = 255;

msg.id = shackbus_sb2id(&sb);

//can2udp(&msg);


}


void power_packet_send(uint8_t addr, uint8_t cmd)
{
	// Send the message to can-bus and can2udp
	can_t enocean_packet;
	/* prio = 3; vlan = 4; src  = 5; dst = 6, prot = 11 */
	enocean_packet.id = ((3L<<26)+(4L<<22)+(5L<<14)+(6L<<6)+11L);
	enocean_packet.flags.rtr = 0;
	enocean_packet.flags.extended = 1;
	enocean_packet.length  = 3;
	enocean_packet.data[0] = 1;
	enocean_packet.data[1] = addr;
	enocean_packet.data[2] = cmd;
	can_send_message(&enocean_packet);
	can2udp(&enocean_packet);
}


void enocean_packet_send(uint8_t addr, uint8_t cmd)
{
	// Send the message to can-bus and can2udp
	can_t enocean_packet;
	/* prio = 3; vlan = 4; src  = 5; dst = 6, prot = 9 */
	enocean_packet.id = ((3L<<26)+(4L<<22)+(5L<<14)+(6L<<6)+9L);
	enocean_packet.flags.rtr = 0;
	enocean_packet.flags.extended = 1;
	enocean_packet.length  = 3;
	enocean_packet.data[1] = cmd;
	enocean_packet.data[0] = addr;
	can_send_message(&enocean_packet);
	can2udp(&enocean_packet);
}


#endif
