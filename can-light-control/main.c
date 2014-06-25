/*----------------------------------------------------------------------------
 Copyright:      Radig Ulrich  mailto: mail@ulrichradig.de
 Author:         Radig Ulrich
 Remarks:        
 known Problems: none
 Version:        24.10.2007
 Description:    Webserver uvm.

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
----------------------------------------------------------------------------*/

#include <avr/io.h>
#include "config.h"
#include "timer.h"
#include <avr/eeprom.h>
#include <util/delay.h>
#include "enocean.h"
#include <avr/interrupt.h>

#include <avr/wdt.h>

#include "can.h"
#include "fifo.h"
#include "uart.h"
#include "enocean_parser.h"
#include "hmi.h"


#include "shackbus.h"


void reset_visualisation(void)
{
led_set(99,1);
_delay_ms(40);
led_set(99,0);
led_set(0,1);
_delay_ms(40);
_delay_ms(20);
_delay_ms(20);
led_set(0,0);
led_set(1,1);
_delay_ms(40);
_delay_ms(20);
_delay_ms(20);
led_set(1,0);
led_set(2,1);
_delay_ms(20);
_delay_ms(40);
_delay_ms(20);
led_set(2,0);
led_set(3,1);
_delay_ms(40);
_delay_ms(20);
_delay_ms(20);
led_set(3,0);
led_set(4,1);
_delay_ms(40);
_delay_ms(20);
_delay_ms(20);
led_set(4,0);
_delay_ms(40);
_delay_ms(20);
}


//----------------------------------------------------------------------------
//Hier startet das Hauptprogramm
int main(void)
{  

	// Configuration hmi (Human Machine Interface)
	#if USE_HMI
		hmi_init();
	#endif

	uart_init();	

	#ifdef UART_DEBUG
		uart_write("\n\rSystem Ready\n\r");
		uart_write("Compiliert am "__DATE__" um "__TIME__"\r\n");
		uart_write("Compiliert mit GCC Version "__VERSION__"\r\n");
	#endif
	_delay_ms(100);
	
	#if USE_ENOCEAN
		enocean_init();
	#endif
	
	timer_init();

	#if USE_SHACKBUS
		shackbus_init();
	#endif

	reset_visualisation();


	//Globale Interrupts einschalten
	sei(); 

	wdt_enable(WDTO_500MS);
	
	while(1)
	{

		wdt_reset();

		#if USE_ENOCEAN
			cli();
			enocean_main();
			sei();
		#endif
		
		#if USE_HMI
			cli();
			hmi_main();
			sei();
		#endif
		
		#if USE_SHACKBUS
			cli();
			shackbus_main();
			sei();
		#endif

		#if USE_ENOCEAN_PARSER
			cli();		
			if ( fifo_get_count(&uart_infifo) > 0 )// Wie groß ist das FIFO?
			{
				if (enocean_parser_poll(fifo_get(&uart_infifo))==ENOCEAN_PARSER_STATE_CHKOK) {
					// Es wurde ein enocean Packet empfangen
					enocean_state_set(enocean_parser_input.id[0],enocean_parser_input.db[3]);			
				}
			}
			sei();
		#endif

		//if (merker)
		//  if(can_check_free_buffer())
		//	can_send_message(&send_test_msg);

		if (merker)
		{
			merker=0;
			static uint8_t wd_flag = 0;
			wd_flag ^= 1;
//            if (wd_flag==1) group_state_set(3,ENOCEAN_CHANNEL_SA_SS);
//            if (wd_flag==0) group_state_set(3,ENOCEAN_CHANNEL_SA_CS);
		}


			cli();		
		can_error_register_t aktuelle_fehler = can_read_error_register();
		if (aktuelle_fehler.rx)
		{
//			group_state_set(2,ENOCEAN_CHANNEL_SA_SS);
		}
		if (aktuelle_fehler.tx)
		{
//			group_state_set(3,ENOCEAN_CHANNEL_SA_SS);
		}
			sei();		

		
		
    }//while (1)
		
return(0);
}

