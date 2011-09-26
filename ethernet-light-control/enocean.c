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



// ----------------------------------------------------------------------------
// default values
#define PORT_DEFAULT			1337

// ----------------------------------------------------------------------------
//global variables
unsigned short enocean_port = PORT_DEFAULT;



uint8_t licht_status[8];
#define ENOCEAN_CHANNEL_STATUS	1
#define ENOCEAN_CHANNEL_ACT		2


enum hauptschalter_states {INIT, OFF, ON, OFF_WAIT};
enum hauptschalter_states hauptschalter_status;

uint8_t blinker;
uint8_t blinker_cnt;




// ----------------------------------------------------------------------------
// initialization of network settings
void enocean_netInit(void) {


 // calculate broadcast adress
 (*((unsigned long*)&broadcast_ip[0])) = (((*((unsigned long*)&myip[0])) & (*((unsigned long*)&netmask[0]))) | (~(*((unsigned long*)&netmask[0]))));

 // remove any existing app from port
 kill_udp_app(enocean_port);
 // add port to stack with callback
 add_udp_app(enocean_port, (void(*)(unsigned char))enocean_get);
}




// ----------------------------------------------------------------------------
// initialization of Art-Net
void enocean_init(void) {

 // read Art-Net port
 eeprom_read_block(&enocean_port, (unsigned char *)ENOCEAN_PORT_EEPROM_STORE, 2);
 if (enocean_port == 0xFFFF) {
  enocean_port = PORT_DEFAULT;
 }

	DDRD  |= 0xF8;	//PD4-PD7 = LEDs  PD3 = Hauptschalter LED
	PORTD &= 0x07;

	DDRC  &= 0xE0;	//PC0-3 = Tastereingänge  PC4 = Hauptschalter
	PORTC |= 0x1F;

	for (uint8_t i = 0; i < 8; i++)
	{
		licht_status[i] = 0;

		licht_status[i] = eeprom_read_byte((unsigned char *)ENOCEAN_LICHT_EEPROM_STORE+i);	

		if (licht_status[i] == 0xFF)
		{
			licht_status[i] = 0;
		}

		licht_status[i] |= ENOCEAN_CHANNEL_ACT;
	}

	hauptschalter_status = INIT;


 return;
}



void send_test_msg(uint8_t addr, uint8_t cmd)
{

	eth_buffer[UDP_DATA_START+0]=addr;
	eth_buffer[UDP_DATA_START+1]=cmd;

	create_new_udp_packet(2, 2342, 2342, (unsigned long)0xffffffff);
	
	return;
}




void enocean_main(void) {

	for (uint8_t i = 0; i < 8; i++) {

		if (licht_status[i] & ENOCEAN_CHANNEL_ACT)
		{
			licht_status[i] &= ~ENOCEAN_CHANNEL_ACT;
			enocean_packet_send(100+i,licht_status[i] & ENOCEAN_CHANNEL_STATUS);	

		    eeprom_write_byte((unsigned char *)ENOCEAN_LICHT_EEPROM_STORE+i, licht_status[i] & ENOCEAN_CHANNEL_STATUS);	
			send_test_msg(i,licht_status[i] & ENOCEAN_CHANNEL_STATUS);
		}

		if (licht_status[i] & ENOCEAN_CHANNEL_STATUS)
		{
			PORTD |= _BV(4+i/2);
		} else {
			if (i%2==0)
			{
				PORTD &= ~_BV(4+i/2);
			}
		}
	
	}

	


	for (uint8_t i = 0; i < 4; i++)
	{
		if ( key_press & _BV(i) )
		{
			key_press ^= _BV(i);
			
			licht_status[i*2] |= ENOCEAN_CHANNEL_ACT;
			licht_status[i*2+1] |= ENOCEAN_CHANNEL_ACT;


			if ( (licht_status[i*2] & ENOCEAN_CHANNEL_STATUS) || (licht_status[i*2+1] & ENOCEAN_CHANNEL_STATUS))
			{
				licht_status[i*2] &= ~(ENOCEAN_CHANNEL_STATUS);
				licht_status[i*2+1] &= ~(ENOCEAN_CHANNEL_STATUS);
			} else {
				licht_status[i*2]   |= ENOCEAN_CHANNEL_STATUS;
				licht_status[i*2+1] |= ENOCEAN_CHANNEL_STATUS;
			}
		}
	}

	{
		if (key_state & _BV(4))		//Abfrage des Hauptschalters true = EIN
		{	
			//Hauptschalter ein
			if (hauptschalter_status == INIT || hauptschalter_status == OFF || hauptschalter_status == OFF_WAIT)
			{
				PORTD |= _BV(PD3);
				enocean_packet_send(110,1);	
				send_test_msg(10,1);
				hauptschalter_status = ON;
			}
		} else {
			//Hauptschalter aus
			if (hauptschalter_status == INIT)
			{
				PORTD &= ~_BV(PD3);
				enocean_packet_send(110,0);	
				send_test_msg(10,0);
				hauptschalter_status = OFF;
			}
			
			if (hauptschalter_status == ON)
			{
				blinker_cnt = 0;
				send_test_msg(10,2);
				hauptschalter_status = OFF_WAIT;
			}

			if (hauptschalter_status == OFF_WAIT)
			{

				if (blinker)
					PORTD |= _BV(PD3);
				else
					PORTD &= ~_BV(PD3);
				
				if (blinker_cnt > 10)
				{
					PORTD &= ~_BV(PD3);
					enocean_packet_send(110,0);	
					send_test_msg(10,0);
					hauptschalter_status = OFF;
				}
			}



		}
	}





		/*if ( key_press & _BV(4) )
		{
			key_press ^= _BV(4);
			
			if ( licht_status[i*2] || licht_status[i*2+1])
			{
				enocean_packet_send(100+i*2, 0 );
				enocean_packet_send(101+i*2, 0 );
			} else {
				enocean_packet_send(100+i*2, 1 );
				enocean_packet_send(101+i*2, 1 );
			}
		}
*/

	
}



// ----------------------------------------------------------------------------
// receive Art-Net packet
void enocean_get(unsigned char index) {

	if(eth_buffer[UDP_DATA_START] == 0xA5 && eth_buffer[UDP_DATA_START+1] == 0x5A)
	{

		uint8_t addr   = eth_buffer[UDP_DATA_START+2];
		uint8_t status = eth_buffer[UDP_DATA_START+3];
		
		if ((addr) < 8 && status <= 1)
		{
			licht_status[addr] |= ENOCEAN_CHANNEL_ACT;
			if (status==0)
				licht_status[addr] &= ~ENOCEAN_CHANNEL_STATUS;
			else	
				licht_status[addr] |= ENOCEAN_CHANNEL_STATUS;
		}

		if ((addr) == 99 && status == 1)
			PORTD |= _BV(PD3);
		if ((addr) == 99 && status == 0)
			PORTD ^= _BV(PD3);

	} else {
	//	enocean_packet_send(100+eth_buffer[UDP_DATA_START]-'0', eth_buffer[UDP_DATA_START+1]-'0' );
	}

}

// ----------------------------------------------------------------------------
// Called by timer, check changes
void enocean_tick(void) {

	static uint8_t counter = 1;
	
	if (!counter--)
	{
		blinker ^= 1;
		counter=0;
		blinker_cnt++;
	}
}




void enocean_packet_send(uint8_t addr, uint8_t cmd)
{
	uint8_t packet_tmp[14] = {0xa5, 0x5a, 0x0b, 0x05, 
		((cmd+2) << 5 | 16),
		0, 0, 0, 0, 0, 0,
		(addr), 0x10, 0};

	for (uint8_t i = 2; i < 13; i++)
		packet_tmp[13] += packet_tmp[i];

	for (uint8_t i = 0; i < 14; i++) {
		usart_write_char(packet_tmp[i]);
	}
}



#endif
