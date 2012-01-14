/*----------------------------------------------------------------------------
 Copyright:      Ulrich Escher  mailto: code@myvdr.de
 Author:         Ulrich Escher
 Remarks:        
 known Problems: none
 Version:        13.01.2012
 Description:    enocean Headerfile

 implement enocean

------------------------------------------------------------------------------*/

#include "enocean.h"

#ifdef USE_ENOCEAN

#include <stdio.h>
#include <avr/interrupt.h>

#include "hmi.h"

uint8_t licht_status[8];

enum hauptschalter_states {INIT, OFF, ON, OFF_WAIT};
enum hauptschalter_states hauptschalter_status;

uint8_t blinker;
uint8_t blinker_cnt;


// ----------------------------------------------------------------------------
// initialization of enocean environment
void enocean_init(void) {

	for (uint8_t i = 0; i < 10; i++)
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





void enocean_main(void) {


	for (uint8_t i = 0; i < 10; i++) {

		if (licht_status[i] & ENOCEAN_CHANNEL_ACT)
		{
			licht_status[i] &= ~ENOCEAN_CHANNEL_ACT;
			enocean_packet_send(100+i,licht_status[i] & ENOCEAN_CHANNEL_STATUS);	
		}


		if (licht_status[i] & ENOCEAN_CHANNEL_EEPROM)
		{
			licht_status[i] &= ~ENOCEAN_CHANNEL_EEPROM;
		    eeprom_write_byte((unsigned char *)ENOCEAN_LICHT_EEPROM_STORE+i, licht_status[i] & ENOCEAN_CHANNEL_STATUS);	
		}

	
	}

	



	{
		if (key_state & _BV(7))		//Abfrage des Hauptschalters true = EIN
		{	
			//Hauptschalter ein
			if (hauptschalter_status == INIT || hauptschalter_status == OFF || hauptschalter_status == OFF_WAIT)
			{
				led_set(99,1);
				enocean_packet_send(110,1);	
				hauptschalter_status = ON;
			}
		} else {
			//Hauptschalter aus
			if (hauptschalter_status == INIT)
			{
				led_set(99,0);
				enocean_packet_send(110,0);	
				hauptschalter_status = OFF;
			}
			
			if (hauptschalter_status == ON)
			{
				blinker_cnt = 0;
				hauptschalter_status = OFF_WAIT;
			}

			if (hauptschalter_status == OFF_WAIT)
			{

				if (blinker)
					led_set(99,1);
				else
					led_set(99,0);
				
				if (blinker_cnt > 5)
				{
					led_set(99,0);
					enocean_packet_send(110,0);	
					hauptschalter_status = OFF;
				}
			}



		}
	}


	
}


// ----------------------------------------------------------------------------
// enocen set function
void enocean_state_set(uint8_t channel, uint8_t state) {
		
		if (channel >= 100) channel -= 100;
		if (state==0x50) state = 0;		
		if (state==0x70) state = 1;		
		if ((channel) < 8 && state <= 1)
		{
			licht_status[channel] |= ENOCEAN_CHANNEL_ACT | ENOCEAN_CHANNEL_EEPROM;
			if (state==0)
				licht_status[channel] &= ~ENOCEAN_CHANNEL_STATUS;
			else	
				licht_status[channel] |= ENOCEAN_CHANNEL_STATUS;
		}

		if (channel == 99)
			led_set(99,state);

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


// ----------------------------------------------------------------------------
// create a enocean telegram and send over uart
void enocean_packet_send(uint8_t addr, uint8_t cmd)
{
	uint8_t packet_tmp[14] = {0xa5, 0x5a, 0x0b, 0x05, 
		((cmd+2) << 5 | 16),
		0, 0, 0, 0, 0, 0,
		(addr), 0x10, 0};

	for (uint8_t i = 2; i < 13; i++) {
		packet_tmp[13] += packet_tmp[i];
	}

	for (uint8_t i = 0; i < 14; i++) {
		uart_putc(packet_tmp[i]);
	}
}


#endif
