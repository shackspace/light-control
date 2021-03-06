/*----------------------------------------------------------------------------
 Copyright:      Radig Ulrich  mailto: mail@ulrichradig.de
 Author:         Radig Ulrich
 Remarks:        
 known Problems: none
 Version:        24.10.2007
 Description:    Webserver uvm.

 Dieses Programm ist freie Software. Sie k�nnen es unter den Bedingungen der 
 GNU General Public License, wie von der Free Software Foundation ver�ffentlicht, 
 weitergeben und/oder modifizieren, entweder gem�� Version 2 der Lizenz oder 
 (nach Ihrer Option) jeder sp�teren Version. 

 Die Ver�ffentlichung dieses Programms erfolgt in der Hoffnung, 
 da� es Ihnen von Nutzen sein wird, aber OHNE IRGENDEINE GARANTIE, 
 sogar ohne die implizite Garantie der MARKTREIFE oder der VERWENDBARKEIT 
 F�R EINEN BESTIMMTEN ZWECK. Details finden Sie in der GNU General Public License. 

 Sie sollten eine Kopie der GNU General Public License zusammen mit diesem 
 Programm erhalten haben. 
 Falls nicht, schreiben Sie an die Free Software Foundation, 
 Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA. 
----------------------------------------------------------------------------*/

#include <avr/io.h>
#include "config.h"
#include "usart.h"
#include "networkcard/enc28j60.h"
#include "networkcard/rtl8019.h"
#include "stack.h"
#include "timer.h"
#include "cmd.h"
#include <avr/eeprom.h>
#include <util/delay.h>
#include "dhcpc.h"
#include "dnsc.h"
#include "enocean.h"
#include "can.h"
#include "can2udp.h"
#include "shackbus.h"

//----------------------------------------------------------------------------
//Hier startet das Hauptprogramm
int main(void)
{  

	//Konfiguration der Ausg�nge bzw. Eing�nge
	//definition erfolgt in der config.h
	//DDRA = OUTA;
	//DDRC = OUTC;
	DDRD = OUTD;
	
    usart_init(BAUDRATE); // setup the UART
	
	usart_write("\n\rSystem Ready\n\r");
    usart_write("Compiliert am "__DATE__" um "__TIME__"\r\n");
    usart_write("Compiliert mit GCC Version "__VERSION__"\r\n");

	_delay_ms(1000);
	
	//Applikationen starten
	stack_init();
	#if USE_ENOCEAN
		enocean_init();
	#endif
	#if USE_CAN2UDP
		can2udp_init();
	#endif
	
	//Ethernetcard Interrupt enable
	ETH_INT_ENABLE;

	//Globale Interrupts einschalten
	sei(); 
	
    #if USE_ENOCEAN
		enocean_netInit();
	#endif
    #if USE_CAN2UDP
		can2udp_netInit();
	#endif

    #if USE_DHCP
    dhcp_init();
    if ( dhcp() == 0)
    {
        save_ip_addresses();
    }
    else
    {
        usart_write("DHCP fail\r\n");
        read_ip_addresses(); //get from EEPROM
    }
    #endif //USE_DHCP
	
    usart_write("\r\nIP   %1i.%1i.%1i.%1i\r\n", myip[0]     , myip[1]     , myip[2]     , myip[3]);
    usart_write("MASK %1i.%1i.%1i.%1i\r\n", netmask[0]  , netmask[1]  , netmask[2]  , netmask[3]);
    usart_write("GW   %1i.%1i.%1i.%1i\r\n", router_ip[0], router_ip[1], router_ip[2], router_ip[3]);

    #if USE_DNS
    usart_write("DNS  %1i.%1i.%1i.%1i\r\n", dns_server_ip[0], dns_server_ip[1], dns_server_ip[2], dns_server_ip[3]);
    #endif //USE_DNS
    
	
	#if USE_SHACKBUS
		shackbus_init();
	#endif


	while(1)
	{


	    eth_get_data();
		
        //Terminalcommandos auswerten
		if (usart_status.usart_ready){
            usart_write("\r\n");
			if(extract_cmd(&usart_rx_buffer[0]))
			{
				usart_write("Ready\r\n\r\n");
			}
			else
			{
				usart_write("ERROR\r\n\r\n");
			}
			usart_status.usart_ready =0;
		}
		
        #if USE_DHCP
        if ( dhcp() != 0) //check for lease timeout
        {
            usart_write("dhcp lease renewal failed\r\n");
			RESET();
        }
        #endif //USE_DHCP
  
		#if USE_SHACKBUS
			cli();
			shackbus_main();
			sei();
		#endif
cli();
		#if USE_ENOCEAN
			enocean_main();
		#endif
		#if USE_CAN2UDP
			can2udp_main();
		#endif
sei();		
        if(ping.result)
        {
            usart_write("Get PONG: %i.%i.%i.%i\r\n",ping.ip1[0],ping.ip1[1],ping.ip1[2],ping.ip1[3]); 
            ping.result = 0;
        }
    }//while (1)
		
return(0);
}

