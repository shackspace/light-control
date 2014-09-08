/*----------------------------------------------------------------------------
 Copyright:      Radig Ulrich  mailto: mail@ulrichradig.de
 Author:         Radig Ulrich
 Remarks:        
 known Problems: none
 Version:        03.11.2007
 Description:    Webserver Config-File

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

#ifndef _CONFIG_H_
	#define _CONFIG_H_	
	

	//ETH_M32_EX (www.ulrichradig.de)
	#define USE_ENC28J60	1
	//Holger Buss (www.mikrocontroller.com) Mega32-Board
	#define USE_RTL8019		0
	
	//Konfiguration der PORTS (HEX)
	//1=OUTPUT / 0=INPUT
	#define OUTA 		0x07
	#define OUTC 		0x00
	#define OUTD 		0x00
	//Achtung!!!! an PORTB ist der ENC
	//nur ändern wenn man weiß was man macht!

    //Watchdog timer for the ENC2860, resets the stack if timeout occurs
    #define WTT 1200 //Watchdog timer in timer interrupt

	//Umrechnung von IP zu unsigned long
	#define IP(a,b,c,d) ((unsigned long)(d)<<24)+((unsigned long)(c)<<16)+((unsigned long)(b)<<8)+a

	//IP des Webservers und des Routers
	//#define MYIP		IP(192,168,0,99)
    #define MYIP		IP(10,42,23,99)
    #define ROUTER_IP	IP(192,168,0,1)

	//Netzwerkmaske
	#define NETMASK		IP(255,255,255,0)
	
	//DHCP-Server
    #define USE_DHCP    1 //1 = DHCP Client on
	
	//ENOCEAN
	#define USE_ENOCEAN 1

	//ENOCEAN
	#define USE_CAN2UDP 1

	#define USE_SHACKBUS 1

	#define USE_FRAMESTORAGE 1

    //DNS-Server
    #define USE_DNS     0 //1 = DNS Client on
    #define DNS_IP      IP(192,168,0,1)
	
	//MAC Adresse des Webservers	
	#define MYMAC1	0x00
	#define MYMAC2	0x22
	#define MYMAC3	0xF9
	#define MYMAC4	0x01
	#define MYMAC5	0x38
	#define MYMAC6	0xc8
	
	//Taktfrequenz
	//#define F_CPU 20000000UL
    #define F_CPU 8000000UL	
	//#define F_CPU 14745600UL
	//#define F_CPU 11059200UL
	
	//Timertakt intern oder extern
	#define EXTCLOCK 0 //0=Intern 1=Externer Uhrenquarz

	//Baudrate der seriellen Schnittstelle
	#define BAUDRATE 9600
	    
#endif //_CONFIG_H


