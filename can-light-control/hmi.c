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

#include <avr/io.h>

#include "config.h"
#include "hmi.h"
#include "enocean.h"

void hmi_init(void)
{
    //LED Ausgänge PC[5-0]	
    PORTC &= 0x3F;
    DDRC = 0x3F;

    //Inputs mit aktiviertem Pullup PD[7-2]	
    DDRD  &= 0x03;
    PORTD |= 0xFC;
}

extern uint8_t licht_status[];
extern void led_set(uint8_t,uint8_t);

uint8_t channel_led_conf[8]={0,1,0,1,2,3,4,4};

uint8_t group_state_get(uint8_t group)
{
	//nachschauen welcher Kanal mit led verknüpft ist.
	for (uint8_t i = 0; i < 8; i++) {		
		if (channel_led_conf[i] == group) {
			if (licht_status[i] & ENOCEAN_CHANNEL_STATUS)
				return 1;
		}
	}
	return 0;
}

void group_state_set(uint8_t group, uint8_t state)
{
	//nachschauen welcher Kanal mit led verknüpft ist.
	for (uint8_t i = 0; i < 8; i++) {		
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
		if ( key_press & _BV(2+j) )
		{
			key_press ^= _BV(2+j);

			if (group_state_get(j))
				group_state_set(j,0);
			else
				group_state_set(j,1);
		}
	
		led_set(j,group_state_get(j));

	}

}

void hmi_tick(void)
{

}

void led_set(uint8_t led, uint8_t status)
{
  #define OUTPUT_PORT PORTC
  #define LED0_ON  OUTPUT_PORT |=  _BV(0)
  #define LED0_OFF OUTPUT_PORT &= ~_BV(0)
  #define LED1_ON  OUTPUT_PORT |=  _BV(1)
  #define LED1_OFF OUTPUT_PORT &= ~_BV(1)
  #define LED2_ON  OUTPUT_PORT |=  _BV(2)
  #define LED2_OFF OUTPUT_PORT &= ~_BV(2)
  #define LED3_ON  OUTPUT_PORT |=  _BV(3)
  #define LED3_OFF OUTPUT_PORT &= ~_BV(3)
  #define LED4_ON  OUTPUT_PORT |=  _BV(4)
  #define LED4_OFF OUTPUT_PORT &= ~_BV(4)
  #define LED5_ON  OUTPUT_PORT |=  _BV(5)
  #define LED5_OFF OUTPUT_PORT &= ~_BV(5)

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

