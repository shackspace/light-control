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
	#define USE_ENOCEANPARSER 1

	//SHACKBUS
	#define USE_SHACKBUS 1

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
	
    
	
// ----------------------------------------------------------------------------
/* Global settings for building the can-lib and application program.
 *
 * The following two #defines must be set identically for the can-lib and
 * your application program. They control the underlying CAN struct. If the
 * settings disagree, the underlying CAN struct will be broken, with
 * unpredictable results.
 * If can.h detects that any of the #defines is not defined, it will set them
 * to the default values shown here, so it is in your own interest to have a
 * consistent setting. Ommiting the #defines in both can-lib and application
 * program will apply the defaults in a consistent way too.
 *
 * Select if you want to use 29 bit identifiers.
 */
#define	SUPPORT_EXTENDED_CANID	1

/* Select if you want to use timestamps.
 * Timestamps are sourced from a register internal to the AT90CAN.
 * Selecting them on any other controller will have no effect, they will
 * be 0 all the time.
 */
#define	SUPPORT_TIMESTAMPS		0
	
	
#endif //_CONFIG_H


