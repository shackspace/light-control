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

#include "can.h"
#include "fifo.h"
#include "uart.h"
#include "enocean_parser.h"
#include "hmi.h"

prog_uint8_t can_filter[] = 
{
	// Group 0
	MCP2515_FILTER(0),				// Filter 0
	MCP2515_FILTER(0),				// Filter 1
	
	// Group 1
	MCP2515_FILTER_EXTENDED(0),		// Filter 2
	MCP2515_FILTER_EXTENDED(0),		// Filter 3
	MCP2515_FILTER_EXTENDED(0),		// Filter 4
	MCP2515_FILTER_EXTENDED(0),		// Filter 5
	
	MCP2515_FILTER(0),				// Mask 0 (for group 0)
	MCP2515_FILTER_EXTENDED(0),		// Mask 1 (for group 1)
};
// You can receive 11 bit identifiers with either group 0 or 1.



//----------------------------------------------------------------------------
//Hier startet das Hauptprogramm
int main(void)
{  



    // Configuration hmi (Human Machine Interface)
    hmi_init();

	uart_init();	

	uart_write("\n\rSystem Ready\n\r");
    uart_write("Compiliert am "__DATE__" um "__TIME__"\r\n");
    uart_write("Compiliert mit GCC Version "__VERSION__"\r\n");

	_delay_ms(100);
	
	#if USE_ENOCEAN
		enocean_init();
	#endif
	
	timer_init();


	//Globale Interrupts einschalten
	sei(); 
	

/*    // Initialize MCP2515
    can_init(BITRATE_125_KBPS);
    uart_write("can_init(BITRATE_125_KBPS);");
	
    // Load filters and masks
    can_static_filter(can_filter);
    uart_write("can_static_filter(can_filter);");
	
    // Create a test messsage
    can_t msg;
	
	msg.id = 0x234567;
	msg.flags.rtr = 0;
	msg.flags.extended = 1;
	
	msg.length = 8;
	msg.data[0] = 0xde;
	msg.data[1] = 0xad;
	msg.data[2] = 0xbe;
	msg.data[3] = 0xef;
	
    uart_write("juhuangekommen1/r/n");
	// Send the message
	can_send_message(&msg);
    uart_write("juhuangekommen2/r/n");
*/
		
	while(1)
	{
		
		#if USE_ENOCEAN
			enocean_main();
		#endif
		
		hmi_main();

		
		cli();		
		if ( fifo_get_count(&uart_infifo) > 0 )// Wie groß ist das FIFO?
		{
			if (enocean_parser_poll(fifo_get(&uart_infifo))==ENOCEAN_PARSER_STATE_CHKOK) {
				// Es wurde ein enocean Packet empfangen
				enocean_state_set(enocean_parser_input.id[0],enocean_parser_input.db[3]);			
			}
		}
		sei();


		if (merker)
		{
/*			uart_write("sendsend/r/n");
			merker=0;
			can_send_message(&msg);

			msg.length++;
			if (msg.length==9)
				msg.length=1;

*/
		}


/*
		// Check if a new messag was received
		if (can_check_message())
		{
			can_t msg;
			
			// Try to read the message
			if (can_get_message(&msg))
			{
				// If we received a message resend it with a different id
				msg.id += 10;
				
				// Send the new message
				can_send_message(&msg);
			}
		}

*/		
    }//while (1)
		
return(0);
}

