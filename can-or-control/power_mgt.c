/*----------------------------------------------------------------------------
 Copyright:      Ulrich Escher  mailto: code@myvdr.de
 Author:         Ulrich Escher
 Remarks:        
 known Problems: none
 Version:        23.05.2014
 Description:    Power Management Headerfile

------------------------------------------------------------------------------*/

#include "config.h"
#if USE_POWER_MGT

#include <avr/io.h>
#include "enocean.h"
#include "shackbus.h"
#include "can.h"


enum power_mgt_states {INIT, OFF, ON};
enum power_mgt_states dusche_state;

uint8_t power_mgt_wait_startup;

#define _DUSCHE_STATE_ ( key_state & _BV(PD2) )

void power_mgt_init(void)
{
	dusche_state = INIT;
	power_mgt_wait_startup = 5;
}


void power_mgt_main(void)
{
	//Hauptschaltermanagement durch Statemachine implementierung
	switch (dusche_state)
	{
		case INIT:
			if ( power_mgt_wait_startup == 0 )
			{
				if ( enocean_state_get(0) ) // 0=>OFF 1=>ON
				{
					dusche_state = ON;
				} else {
					dusche_state = OFF;
				}
			}
			break;

		case OFF:
			if ( _DUSCHE_STATE_ )
			{
				enocean_state_set(0,ENOCEAN_CHANNEL_SE_SA_SS);
				dusche_state = ON;
			}
			break;

		case ON:
			if ( !_DUSCHE_STATE_ )
			{
				enocean_state_set(0,ENOCEAN_CHANNEL_SE_SA_CS);
				dusche_state = OFF;
			}
			break;

		default:
			dusche_state = INIT;
			break;
	}

}


void power_mgt_tick(void)
{
	can_t ka;
	shackbus_id_t ka_id;
	ka_id.prio = 1;
	ka_id.vlan = 2;
	ka_id.dst  = 255;
	ka_id.src  = 8;
	ka_id.prot = 12;

	ka.id = shackbus_sb2id(&ka_id);
	ka.length = 5;

	if (power_mgt_wait_startup)
	{
		power_mgt_wait_startup--;
	}

	ka.data[0] = dusche_state;
	ka.data[1] = power_mgt_wait_startup;
	ka.data[2] = enocean_state_get(0);
	ka.data[3] = _DUSCHE_STATE_;
	ka.data[4] = eeprom_read_byte((unsigned char *)ENOCEAN_CHANNEL_EEPROM_STORE+0);

	can_send_message(&ka);

}


#endif // USE_POWER_MGT
