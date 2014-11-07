/*----------------------------------------------------------------------------
 Copyright:      Ulrich Escher  mailto: code@myvdr.de
 Author:         Ulrich Escher
 Remarks:        
 known Problems: none
 Version:        13.01.2012
 Description:    config file

------------------------------------------------------------------------------*/

#ifndef _CONFIG_H_
	#define _CONFIG_H_

	//ENOCEAN
	#define USE_ENOCEAN 1

	//HMI
	#define USE_HMI 1

	//ENOCEANPARSER
	#define USE_ENOCEAN_PARSER 0

	//SHACKBUS
	#define USE_SHACKBUS 1

	#define USE_FRAMESTORAGE 1

	//Taktfrequenz
	//#define F_CPU 20000000UL
	//#define F_CPU 8000000UL
	#define F_CPU 14745600UL
	//#define F_CPU 8000000UL
	//#define F_CPU 11059200UL

	//Timertakt intern oder extern
	#define EXTCLOCK 0 //0=Intern 1=Externer Uhrenquarz

	//Baudrate der seriellen Schnittstelle
	#define BAUDRATE 9600

#endif //_CONFIG_H
