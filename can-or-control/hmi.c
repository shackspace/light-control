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

extern uint8_t enocean_channel_state[];
void led_set(uint8_t,uint8_t);

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


void hmi_main(void)
{
	//für jede LED einen Durchlauf	
	for (uint8_t j = 0; j < 5; j++)
	{
		if ( key_press & _BV(j) )
		{
			key_press ^= _BV(j);
		}
	
		led_set(j,0);

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

}


void hmi_tick(void)
{
	static uint8_t counter = 1;
	
	if (!counter--)
	{
		blinker ^= 1;
		counter=0;
		blinker_cnt++;
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
