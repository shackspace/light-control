/*----------------------------------------------------------------------------
 Copyright:      Radig Ulrich  mailto: mail@ulrichradig.de
 Author:         Radig Ulrich
 Remarks:
 known Problems: none
 Version:        24.10.2007
 Description:    Timer Routinen

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

#include "config.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

#if USE_ENOCEAN
	#include "enocean.h"
#endif //USE_ENOCEAN

#if USE_HMI
	#include "hmi.h"
#endif //USE_HMI

#if USE_POWER_MGT
	#include "power_mgt.h"
#endif //USE_POWER_MGT

#if USE_SHACKBUS
	#include "shackbus.h"
#endif //USE_SHACKBUS


volatile uint8_t key_state = 0;                                // debounced and inverted key state:
volatile uint8_t key_press = 0;                                // key press detect

volatile uint8_t merker = 0;

volatile system_time_t system_time;		//100ms tick since startup


//----------------------------------------------------------------------------
//Diese Routine startet und inizialisiert den Timer
void timer_init (void)
{
	#if EXTCLOCK==1
		#if defined (__AVR_ATmega644__)
			//Asynchroner Modus ein, Oszillator an TOSC1 und TOSC2 aktiv
			ASSR |= (1<<AS2);
			TCCR2B = 0x05;
			while(ASSR & 0x11);
			//Capture/Compare-Interrupt aktiv
			TIMSK2 |= (1<<OCIE2A);
		#else
			//Asynchroner Modus ein, Oszillator an TOSC1 und TOSC2 aktiv
			ASSR  = (1<<AS2);
			//CTC-Modus an (Clear Timer on Compare Match)
			TCCR2 = (1<<WGM21);			
			//dieser Wert ergibt eine Sekunde Periodendauer
			OCR2  = 31;	
			//lösche Prescaler 2				
			SFIE  = (1<<PSR2);			
			//Starte Timer 2 mit Prescaler gleich 1/1024
			TCCR2 |= (1<<CS22)|(1<<CS21)|(1<<CS20); 
			while(ASSR & 0x07);
			//Capture/Compare-Interrupt aktiv
			TIMSK = (1<<OCIE2);			
		#endif
	#else
		#if defined (__AVR_ATmega328P__)
			TCCR1B |= (1<<WGM12) | (1<<CS10 | 0<<CS11 | 1<<CS12); //1024
			TCNT1 = 0;
			OCR1A = ((F_CPU / 1024) / 200) - 1;
			TIMSK1 = (1 << OCIE1A);
		#else
			TCCR1B |= (1<<WGM12) | (1<<CS10 | 0<<CS11 | 1<<CS12); //1024
			TCNT1 = 0;
			OCR1A = ((F_CPU / 1024) / 200) - 1;
			TIMSK |= (1 << OCIE1A);
		#endif
	#endif
	return;
}




//----------------------------------------------------------------------------
//Timer Interrupt
#if EXTCLOCK==1
	#if defined (__AVR_ATmega644__)
    ISR (TIMER2_COMPA_vect)
	#else
    ISR (TIMER2_COMP_vect)
	#endif
#else
	ISR (TIMER1_COMPA_vect)
#endif
{

	if(uart_frameing > 0) uart_frameing--;

	static uint16_t    prescaler_10ms = 2;
	if(!prescaler_10ms--)
	{
		prescaler_10ms = 1;

		static uint8_t ct0, ct1;
		uint8_t i;
		
		uint8_t input_byte = 0;

		input_byte |= ((PIND<<3)&0xE0); 

		input_byte |= ((PIND>>1)&0x10); 
		input_byte |= ((PIND>>3)&0x08); 
		input_byte |= ((PIND>>5)&0x04); 
		input_byte |= ((PINB<<1)&0x02); 
		input_byte |= ((PIND>>2)&0x01); 
		 
		i = key_state ^ ~input_byte;                            // key changed ?

		ct0 = ~( ct0 & i );                             // reset or count ct0
		ct1 = ct0 ^ (ct1 & i);                          // reset or count ct1
		i &= ct0 & ct1;                                 // count until roll over ?
		key_state ^= i;                                 // then toggle debounced state
		key_press |= key_state & i;                     // 0->1: key press detect

	}

	static uint16_t    prescaler_s = 200;
	if(!prescaler_s--)
	{
		prescaler_s = 199;
		merker = 1;

		#if USE_ENOCEAN
			enocean_tick();
		#endif //USE_ENOCEAN

		#if USE_HMI
			hmi_tick();
		#endif //USE_HMI

		#if USE_POWER_MGT
			power_mgt_tick();
		#endif //USE_POWER_MGT

		#if USE_SHACKBUS
			shackbus_tick();
		#endif //USE_SHACKBUS

	}

	static uint16_t    prescaler_100ms = 333;
	if(!prescaler_100ms--)
	{
		prescaler_100ms = 19;

		system_time.type64++;
	}
}

