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



// ----------------------------------------------------------------------------
// packet formats
struct enocean_packet {
 uint8_t  data[14];
};

uint8_t lichtstatus_tmp[8];
#define ENOCEAN_CHANNEL_STATUS	1
#define ENOCEAN_CHANNEL_ACT		2




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

DDRD  |= 0xF0;
PORTD &= 0x0F;

DDRC  &= 0xF0;
PORTC |= 0x0F;

for (uint8_t i = 0; i < 8; i++)
{
	lichtstatus_tmp[i] = 0;

	lichtstatus_tmp[i] = eeprom_read_byte((unsigned char *)ENOCEAN_LICHT_EEPROM_STORE+i);	

	if (lichtstatus_tmp[i] == 0xFF)
	{
		lichtstatus_tmp[i] = 0;
	}

	lichtstatus_tmp[i] |= ENOCEAN_CHANNEL_ACT;


}

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

		if (lichtstatus_tmp[i] & ENOCEAN_CHANNEL_ACT)
		{
			lichtstatus_tmp[i] &= ~ENOCEAN_CHANNEL_ACT;
			enocean_packet_send(100+i,lichtstatus_tmp[i] & ENOCEAN_CHANNEL_STATUS);	

		    eeprom_write_byte((unsigned char *)ENOCEAN_LICHT_EEPROM_STORE+i, lichtstatus_tmp[i] & ENOCEAN_CHANNEL_STATUS);	
			send_test_msg(100+i,lichtstatus_tmp[i] & ENOCEAN_CHANNEL_STATUS);
		}
	
	}

	PORTD &= 0x0F;
	if (lichtstatus_tmp[0] & ENOCEAN_CHANNEL_STATUS)
		PORTD |= _BV(4);
	if (lichtstatus_tmp[1] & ENOCEAN_CHANNEL_STATUS)
		PORTD |= _BV(4);
	if (lichtstatus_tmp[2] & ENOCEAN_CHANNEL_STATUS)
		PORTD |= _BV(5);
	if (lichtstatus_tmp[3] & ENOCEAN_CHANNEL_STATUS)
		PORTD |= _BV(5);
	if (lichtstatus_tmp[4] & ENOCEAN_CHANNEL_STATUS)
		PORTD |= _BV(6);
	if (lichtstatus_tmp[5] & ENOCEAN_CHANNEL_STATUS)
		PORTD |= _BV(6);
	if (lichtstatus_tmp[6] & ENOCEAN_CHANNEL_STATUS)
		PORTD |= _BV(7);
	if (lichtstatus_tmp[7] & ENOCEAN_CHANNEL_STATUS)
		PORTD |= _BV(7);



	for (uint8_t i = 0; i < 4; i++)
	{
		if ( key_press & _BV(i) )
		{
			key_press ^= _BV(i);
			
			lichtstatus_tmp[i*2] |= ENOCEAN_CHANNEL_ACT;
			lichtstatus_tmp[i*2+1] |= ENOCEAN_CHANNEL_ACT;


			if ( (lichtstatus_tmp[i*2] & ENOCEAN_CHANNEL_STATUS) || (lichtstatus_tmp[i*2+1] & ENOCEAN_CHANNEL_STATUS))
			{
				lichtstatus_tmp[i*2] &= ~(ENOCEAN_CHANNEL_STATUS);
				lichtstatus_tmp[i*2+1] &= ~(ENOCEAN_CHANNEL_STATUS);
			} else {
				lichtstatus_tmp[i*2]   |= ENOCEAN_CHANNEL_STATUS;
				lichtstatus_tmp[i*2+1] |= ENOCEAN_CHANNEL_STATUS;
			}
		}
	}

		/*if ( key_press & _BV(4) )
		{
			key_press ^= _BV(4);
			
			if ( lichtstatus_tmp[i*2] || lichtstatus_tmp[i*2+1])
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
		
		if ((addr) > 8 || status > 1)	return;

		lichtstatus_tmp[eth_buffer[UDP_DATA_START+2]] |= ENOCEAN_CHANNEL_ACT;
		if (eth_buffer[UDP_DATA_START+3]==0)
			lichtstatus_tmp[eth_buffer[UDP_DATA_START+2]] &= ~ENOCEAN_CHANNEL_STATUS;
		else
			lichtstatus_tmp[eth_buffer[UDP_DATA_START+2]] |= ENOCEAN_CHANNEL_STATUS;
		

	} else {
	//	enocean_packet_send(100+eth_buffer[UDP_DATA_START]-'0', eth_buffer[UDP_DATA_START+1]-'0' );
	}

}

// ----------------------------------------------------------------------------
// Called by timer, check changes
void enocean_tick(void) {

}

// ----------------------------------------------------------------------------
// DMX transmission
#if defined(__AVR_ATmega328P__)
ISR (USART_TX_vect) {
#else
ISR (USART_TXC_vect) {
#endif
}

// ----------------------------------------------------------------------------
// DMX reception
#if defined(__AVR_ATmega328P__)
ISR (USART_RX_vect)
#else
ISR (USART_RXC_vect)
#endif
{
#if defined(__AVR_ATmega328P__)
 unsigned char status = UCSR0A; 	// status register must be read prior to UDR (because of 2 byte fifo buffer)
 unsigned char byte = UDR0; 		// immediately catch data from i/o register to enable reception of the next byte
#else
 unsigned char status = UCSRA; 	// status register must be read prior to UDR (because of 2 byte fifo buffer)
 unsigned char byte = UDR; 		// immediately catch data from i/o register to enable reception of the next byte
#endif
byte++;
status++;
}


void enocean_packet_send(uint8_t addr, uint8_t cmd)
{
	uint8_t packet_tmp[14] = {0xa5, 0x5a, 0x0b, 0x05, 0, 0, 0, 0, 0, 0, 0, 0, 0x10, 0};

	packet_tmp[4] = ((cmd+2) << 5 | 16);
	packet_tmp[11] = (addr);

	for (uint8_t i = 2; i < 13; i++)
		packet_tmp[13] += packet_tmp[i];

	for (uint8_t i = 0; i < 14; i++) {
		usart_write_char(packet_tmp[i]);
	}
}



#endif
