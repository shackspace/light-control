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
#include "power_mgt.h"

#include "shackbus.h"

//----------------------------------------------------------------------------
//Hier startet das Hauptprogramm
int main(void)
{  
	// Set Output LED1 LED2
	DDRC  |= (1<<PC3)|(1<<PC2);

	// Set Output SPI SS Pin
	DDRB = (1<<PB2);

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

	#if USE_POWER_MGT
		power_mgt_init();
	#endif

	//Globale Interrupts einschalten
	sei(); 

	wdt_enable(WDTO_500MS);
	
	// Activate LED1 - System OK
	PORTC = (1<<PC3);

	while(1)
	{

		wdt_reset();

		#if USE_ENOCEAN
			enocean_main();
			cli();
			uart_main();
			sei();
		#endif
		
		#if USE_HMI
			cli();
			hmi_main();
			sei();
		#endif
		
		#if USE_POWER_MGT
			cli();
			power_mgt_main();
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

    }//while (1)
		
return(0);
}

