/*----------------------------------------------------------------------------
 Copyright:      Ulrich Escher  mailto: code@myvdr.de
 Author:         Ulrich Escher
 Remarks:        
 known Problems: none
 Version:        13.01.2012 initial
                 13.04.2012 upgrade Channels
 Description:    enocean

 implement enocean

------------------------------------------------------------------------------*/

#include "config.h"

#if USE_ENOCEAN

#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "enocean.h"
#include "hmi.h"
#include "shackbus.h"

uint8_t enocean_channel_state[ENOCEAN_CHANNEL_COUNT];


// ----------------------------------------------------------------------------
// initialization of enocean environment
void enocean_init(void) {

	for (uint8_t i = 0; i < ENOCEAN_CHANNEL_COUNT; i++)
	{
		enocean_channel_state[i] = 0;

		enocean_channel_state[i] = eeprom_read_byte((unsigned char *)ENOCEAN_CHANNEL_EEPROM_STORE+i);	

		if (enocean_channel_state[i] == 0xFF)
		{
			enocean_channel_state[i] = 0;
		}

		enocean_channel_state[i] |= ENOCEAN_CHANNEL_ACT;
	}


 return;
}


// ----------------------------------------------------------------------------
// enocean_main function wird von while(1) aufgerufen
void enocean_main(void) {

	for (uint8_t i = 0; i < ENOCEAN_CHANNEL_COUNT; i++) {

		if (enocean_channel_state[i] & ENOCEAN_CHANNEL_ACT)
		{
			uint8_t ret = enocean_packet_send(ENOCEAN_CHANNEL_OFFSET+i,enocean_channel_state[i] & ENOCEAN_CHANNEL_STATUS);
			if (ret == true)
			{
				enocean_channel_state[i] &= ~ENOCEAN_CHANNEL_ACT;
			}
		}

		if (enocean_channel_state[i] & ENOCEAN_CHANNEL_EEPROM)
		{
			enocean_channel_state[i] &= ~ENOCEAN_CHANNEL_EEPROM;
			eeprom_write_byte((unsigned char *)ENOCEAN_CHANNEL_EEPROM_STORE+i, enocean_channel_state[i] & ENOCEAN_CHANNEL_STATUS);	
		}
	
	}
	
}


// ----------------------------------------------------------------------------
// enocen set function
void enocean_state_set(uint8_t channel, uint8_t state) {
		
  if (channel >= ENOCEAN_CHANNEL_OFFSET) channel -= ENOCEAN_CHANNEL_OFFSET;	
  if ((channel) < ENOCEAN_CHANNEL_COUNT)
  {
    if (state <= 1)
    {
      enocean_channel_state[channel] |= ENOCEAN_CHANNEL_ACT | ENOCEAN_CHANNEL_EEPROM;
			if (state==0)
				enocean_channel_state[channel] &= ~ENOCEAN_CHANNEL_STATUS;
			else	
				enocean_channel_state[channel] |= ENOCEAN_CHANNEL_STATUS;
    }
    if (state & 0x80)
    {
      uint8_t mask = (state&0x70)>>4;
      uint8_t bits = (state&0x07);
      enocean_channel_state[channel] = (enocean_channel_state[channel] & ~mask) | ( mask & bits );
	}

    //Event über Shackbus senden
    shackbus_send_msg(channel+ENOCEAN_CHANNEL_OFFSET, enocean_channel_state[channel] & ENOCEAN_CHANNEL_STATUS);
  }

}


// ----------------------------------------------------------------------------
// enocen get function
uint8_t enocean_state_get(uint8_t channel) {
  if ( enocean_channel_state[channel] & ENOCEAN_CHANNEL_STATUS )
    return 1;
  else
    return 0;
}


// ----------------------------------------------------------------------------
// Called by timer, check changes
void enocean_tick(void) {

}

// ----------------------------------------------------------------------------
// create a enocean telegram and send over uart
uint8_t enocean_packet_send(uint8_t addr, uint8_t cmd)
{
	uint8_t packet_tmp[14] = {0xa5, 0x5a, 0x0b, 0x05, 
		((cmd+2) << 5 | 16),
		0, 0, 0, 0, 0, 0,
		(addr), 0x10, 0};

	if (uart_size_free_buffer_tx() < sizeof(packet_tmp))
		return false;

	for (uint8_t i = 2; i < 13; i++) {
		packet_tmp[13] += packet_tmp[i];
	}

	uint8_t sreg = SREG;
	cli();

	for (uint8_t i = 0; i < 14; i++) {
		uart_putc(packet_tmp[i]);
	}

	SREG = sreg;

	return true;
}


#endif
