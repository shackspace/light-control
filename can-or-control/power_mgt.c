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
#include "power_mgt.h"


enum power_mgt_states_enum {INIT, OFF, ON, DO_ON, DO_OFF, OFF_WAIT};

typedef struct
{
	enum power_mgt_states_enum state;
	uint8_t wait_startup;
	uint8_t input_1;
	uint16_t wait_off;
	uint16_t counter;
} power_mgt_state_t;

power_mgt_state_t power_mgt_state[POWER_MGT_CHANNEL_COUNT];

power_mgt_state_t* state;

#define OUTPUT_ON  enocean_state_set(i,ENOCEAN_CHANNEL_SE_SA_SS);
#define OUTPUT_OFF enocean_state_set(i,ENOCEAN_CHANNEL_SE_SA_CS);
#define STATE      state->state

void power_mgt_set_input_1(uint8_t _channel, uint8_t _state)
{
	power_mgt_state[_channel].input_1 = _state;
	return;
}

void power_mgt_set_wait_off(uint8_t _channel, uint16_t _value)
{
	power_mgt_state[_channel].wait_off = _value;
	return;
}

void power_mgt_init(void)
{
	for (uint8_t i=0;i<POWER_MGT_CHANNEL_COUNT;i++)
	{
		power_mgt_state[i].state = INIT;
		power_mgt_state[i].wait_startup = 5-i;
		power_mgt_state[i].input_1 = 0;
		power_mgt_state[i].wait_off = 0;
		power_mgt_state[i].counter = power_mgt_state[i].wait_startup;
	}
}


void power_mgt_main(void)
{
	power_mgt_state[0].input_1 = key_state & _BV(PD2);

	static uint8_t i = POWER_MGT_CHANNEL_COUNT;

	if (++i >= POWER_MGT_CHANNEL_COUNT) i = 0;
	state = &power_mgt_state[i];

	//Power Management with Statemachine Implementation
	switch (STATE)
	{
		case INIT:
			if ( state->counter == 0 )
			{
				if ( enocean_state_get(i) ) // 0=>OFF 1=>ON
					STATE = ON;
				else
					STATE = OFF;
			}
			break;


		case DO_OFF:
			OUTPUT_OFF;
			STATE = OFF;

		case OFF:
			if ( state->input_1 )
				STATE = DO_ON;
			break;


		case DO_ON:
			OUTPUT_ON;
			STATE = ON;

		case ON:
			if ( state->input_1 ) //ON
			{
				break;
			} else { //OFF
				if ( state->wait_off)
					state->counter = state->wait_off;
				STATE = OFF_WAIT;
			}

		case OFF_WAIT:
			if ( !state->input_1 ) //Off
			{
				if ( state->counter == 0 )
					STATE = DO_OFF;
			} else
				STATE = DO_ON;
			break;

		default:
			STATE = INIT;
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

	for (int i=0;i<POWER_MGT_CHANNEL_COUNT;i++)
	{
		if (power_mgt_state[i].counter) power_mgt_state[i].counter--;
	}

	ka.data[0] = power_mgt_state[0].state;
	ka.data[1] = power_mgt_state[0].counter;
	ka.data[2] = enocean_state_get(0);
	ka.data[3] = power_mgt_state[0].input_1;
	ka.data[4] = eeprom_read_byte((unsigned char *)ENOCEAN_CHANNEL_EEPROM_STORE+0);

	can_send_message(&ka);

}


#endif // USE_POWER_MGT
