/*----------------------------------------------------------------------------
 Copyright:      Ulrich Escher  mailto: code@myvdr.de
 Author:         Ulrich Escher
 Remarks:        
 known Problems: none
 Version:        13.01.2012
 Description:    Human Machine Interface

 User Interface:
  - LED-Control
  - Input reading

------------------------------------------------------------------------------*/

#include "config.h"
#if USE_HMI

#include <avr/io.h>
#include "hmi.h"
#include "enocean.h"
#include "shackbus.h"


enum hauptschalter_states {INIT, OFF, ON, OFF_WAIT};
enum hauptschalter_states hauptschalter_status;

uint8_t blinker;
uint8_t blinker_cnt;

uint8_t flag_light_shutdown;

extern uint8_t enocean_channel_state[];
void led_set(uint8_t,uint8_t);

//uint8_t channel_led_conf[8]={0,1,0,1,3,4,4,2};
uint8_t channel_led_conf[12]={0,0,1,1,0,1,3,3,4,4,2,5};


void hmi_init(void)
{
    //LED Ausgänge PC[5-0]	
    PORTC &= 0x3F;
    DDRC = 0x3F;

    //Inputs mit aktiviertem Pullup PD[7-2]	
    DDRD  &= 0x03;
    PORTD |= 0xFC;

	DDRB  &= 0xFE;
	PORTB |= 0x01;

	hauptschalter_status = INIT;

}


uint8_t group_state_get(uint8_t group)
{
	//nachschauen welcher Kanal mit led verknüpft ist.
	for (uint8_t i = 0; i < 12; i++) {		
		if (channel_led_conf[i] == group) {
			if (enocean_channel_state[i] & ENOCEAN_CHANNEL_STATUS)
				return 1;
		}
	}
	return 0;
}

void group_state_set(uint8_t group, uint8_t state)
{
	//nachschauen welcher Kanal mit led verknüpft ist.
	for (uint8_t i = 0; i < 12; i++) {		
		if (channel_led_conf[i] == group) {
			enocean_state_set(i, state);
		}
	}
	return;
}

void hmi_main(void)
{
	//für jede LED einen Durchlauf	
	for (uint8_t j = 0; j < 5; j++)
	{
		if ( key_press & _BV(j) )
		{
			key_press ^= _BV(j);
			if (group_state_get(j))
				group_state_set(j,ENOCEAN_CHANNEL_SE_SA_CS);
			else
				group_state_set(j,ENOCEAN_CHANNEL_SE_SA_SS);
		}
	
		led_set(j,group_state_get(j));

	}


	//Hauptschaltermanagement durch Statemachine implementierung
	switch (hauptschalter_status)
	{
		case INIT:
			if (MAINSWITCH_STATE)		//Abfrage des Hauptschalters true = EIN
			{	
				led_set(99,1);
				enocean_packet_send(120,1);	
				shackbus_send_msg(120, 1);
				hauptschalter_status = ON;
			} else {
				led_set(99,0);
				enocean_packet_send(120,0);	
				shackbus_send_msg(120, 0);
				hauptschalter_status = OFF;
			}
			break;

		case OFF:
			if (MAINSWITCH_STATE)
			{
				led_set(99,1);
				enocean_packet_send(120,1);	
				shackbus_send_msg(120, 1);
				hauptschalter_status = ON;
			}
			break;

		case ON:
			if (!MAINSWITCH_STATE)
			{
				blinker_cnt = 0;
				shackbus_send_msg(120, 2);
				hauptschalter_status = OFF_WAIT;
			}
			break;

		case OFF_WAIT:
			if (!MAINSWITCH_STATE)
			{
				if (blinker)
					led_set(99,1);
				else
					led_set(99,0);
			
				if (blinker_cnt > 5)
				{
					led_set(99,0);
					enocean_packet_send(120,0);	
					shackbus_send_msg(120, 0);
					hauptschalter_status = OFF;
				}
			} else {
				led_set(99,1);
				enocean_packet_send(120,1);	
				shackbus_send_msg(120, 1);
				hauptschalter_status = ON;
			}
			break;



		default:		
			break;	
	}

	if (flag_light_shutdown)
	{
		flag_light_shutdown = 0;
		hmi_light_shutdown();
	}


}

void hmi_light_shutdown(void)
{
	static uint8_t light_shutdown_state = 0;
	static uint8_t light_shutdown_counter = 0;

	if ( hauptschalter_status==ON )
		light_shutdown_state=0; 

	if (light_shutdown_state==0)
	{
		light_shutdown_counter=0;
	}

	
	if ( light_shutdown_state==0 && hauptschalter_status==OFF_WAIT )
	{
		light_shutdown_state = 1;
	}	

	if (light_shutdown_state==1)
	{
		if (light_shutdown_counter == 1)
		{
			if ( group_state_get(0) || group_state_get(1) || group_state_get(3) || group_state_get(4) )
				group_state_set(2,ENOCEAN_CHANNEL_SE_SA_SS);
		}		
		if (light_shutdown_counter == 2)
			enocean_state_set(0,ENOCEAN_CHANNEL_SE_SA_CS);
		if (light_shutdown_counter == 3)
			enocean_state_set(1,ENOCEAN_CHANNEL_SE_SA_CS);
		if (light_shutdown_counter == 4)
			enocean_state_set(2,ENOCEAN_CHANNEL_SE_SA_CS);
		if (light_shutdown_counter == 5)
			enocean_state_set(3,ENOCEAN_CHANNEL_SE_SA_CS);
		if (light_shutdown_counter == 6)
			enocean_state_set(4,ENOCEAN_CHANNEL_SE_SA_CS);
		if (light_shutdown_counter == 7)
			enocean_state_set(5,ENOCEAN_CHANNEL_SE_SA_CS);
		if (light_shutdown_counter == 6)
			enocean_state_set(6,ENOCEAN_CHANNEL_SE_SA_CS);
		if (light_shutdown_counter == 7)
			enocean_state_set(7,ENOCEAN_CHANNEL_SE_SA_CS);
		if (light_shutdown_counter == 8)
			enocean_state_set(8,ENOCEAN_CHANNEL_SE_SA_CS);
		if (light_shutdown_counter == 9)
			enocean_state_set(9,ENOCEAN_CHANNEL_SE_SA_CS);

		if (light_shutdown_counter == 15)
		{
			group_state_set(2,ENOCEAN_CHANNEL_SE_SA_CS);
		}		

		light_shutdown_counter++;
	}

	if( light_shutdown_counter >= 30 )
		light_shutdown_state = 0;


}


void hmi_tick(void)
{
	static uint8_t counter = 1;
	
	if (!counter--)
	{
		blinker ^= 1;
		counter=0;
		blinker_cnt++;

		flag_light_shutdown = 1;

	}

}

void led_set(uint8_t led, uint8_t status)
{
  #define OUTPUT_PORT PORTC
  #define LED0_ON  OUTPUT_PORT |=  _BV(5)
  #define LED0_OFF OUTPUT_PORT &= ~_BV(5)
  #define LED1_ON  OUTPUT_PORT |=  _BV(4)
  #define LED1_OFF OUTPUT_PORT &= ~_BV(4)
  #define LED2_ON  OUTPUT_PORT |=  _BV(3)
  #define LED2_OFF OUTPUT_PORT &= ~_BV(3)
  #define LED3_ON  OUTPUT_PORT |=  _BV(2)
  #define LED3_OFF OUTPUT_PORT &= ~_BV(2)
  #define LED4_ON  OUTPUT_PORT |=  _BV(1)
  #define LED4_OFF OUTPUT_PORT &= ~_BV(1)
  #define LED5_ON  OUTPUT_PORT |=  _BV(0)
  #define LED5_OFF OUTPUT_PORT &= ~_BV(0)
	
  if (led==0 && status==1) LED1_ON;
  if (led==0 && status==0) LED1_OFF;
  if (led==1 && status==1) LED2_ON;
  if (led==1 && status==0) LED2_OFF;
  if (led==2 && status==1) LED3_ON;
  if (led==2 && status==0) LED3_OFF;
  if (led==3 && status==1) LED4_ON;
  if (led==3 && status==0) LED4_OFF;
  if (led==4 && status==1) LED5_ON;
  if (led==4 && status==0) LED5_OFF;

  if (led==99&& status==1) LED0_ON;
  if (led==99&& status==0) LED0_OFF;
}

#endif // USE_HMI
