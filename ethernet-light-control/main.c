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
#include "wol.h"
#include "httpd.h"
#include "cmd.h"
#include "telnetd.h"
#include "base64.h"
#include "http_get.h"
#include "lcd.h"
#include "udp_lcd.h"
#include "analog.h"
#include "camera/cam.h"
#include "camera/servo.h"
#include <avr/eeprom.h>
#include <util/delay.h>
#include "dhcpc.h"
#include "dnsc.h"
#include "enocean.h"
#include "can.h"
#include "can2udp.h"

//----------------------------------------------------------------------------
//Hier startet das Hauptprogramm
int main(void)
{  

	//Konfiguration der Ausg�nge bzw. Eing�nge
	//definition erfolgt in der config.h
	//DDRA = OUTA;
	//DDRC = OUTC;
	DDRD = OUTD;
	
 	#if USE_SERVO
		servo_init ();
	#endif //USE_SERVO
	
    usart_init(BAUDRATE); // setup the UART
	
	#if USE_ADC
		ADC_Init();
	#endif
	
	usart_write("\n\rSystem Ready\n\r");
    usart_write("Compiliert am "__DATE__" um "__TIME__"\r\n");
    usart_write("Compiliert mit GCC Version "__VERSION__"\r\n");

	_delay_ms(1000);
	
	//Applikationen starten
	stack_init();
	#if USE_HTTPD
		httpd_init();
	#endif   
	#if USE_TELNETD
		telnetd_init();
	#endif
	#if USE_ENOCEAN
		enocean_init();
	#endif
	#if USE_CAN2UDP
		can2udp_init();
	#endif
	
	//Spielerrei mit einem LCD
	#if USE_SER_LCD
	udp_lcd_init();
	lcd_init();
	lcd_clear();
	back_light = 1;
	lcd_print(0,0,"System Ready");
	#endif
	//Ethernetcard Interrupt enable
	ETH_INT_ENABLE;

	//Globale Interrupts einschalten
	sei(); 
	
	#if USE_CAM
		#if USE_SER_LCD
		lcd_print(1,0,"CAMERA INIT");
		#endif //USE_SER_LCD
	for(a=0;a<2000000;a++){asm("nop");};
	cam_init();
	max_bytes = cam_picture_store(CAM_RESOLUTION);
		#if USE_SER_LCD
		back_light = 0;
		lcd_print(1,0,"CAMERA READY");
		#endif //USE_SER_LCD
	#endif //USE_CAM

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
    
	
	#if USE_WOL
        wol_init();
	#endif //USE_WOL
    

	
	// initialisieren des MCP2515
	can_init(BITRATE_125_KBPS);
	usart_write("can_init(BITRATE_125_KBPS)\r\n");
//	can_set_mode(LOOPBACK_MODE);


	while(1)
	{


		#if USE_ADC
		ANALOG_ON;
		#endif
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
		
        //Wetterdaten empfangen (Testphase)
        #if GET_WEATHER
        http_request ();
        #endif
        
        //Rechner im Netzwerk aufwecken
        #if USE_WOL
        if (wol_enable == 1)
        {
            wol_enable = 0;
            wol_request();
        }
        #endif //USE_WOL
        
        #if USE_DHCP
        if ( dhcp() != 0) //check for lease timeout
        {
            usart_write("dhcp lease renewal failed\r\n");
			RESET();
        }
        #endif //USE_DHCP
  
		//USART Daten f�r Telnetanwendung?
		#if USE_TELNETD
			telnetd_send_data();
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

