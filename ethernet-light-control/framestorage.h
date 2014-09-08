/*----------------------------------------------------------------------------
 Copyright:      Ulrich Escher  mailto: code@myvdr.de
 Author:         Ulrich Escher
 Remarks:        
 known Problems: none
 Version:        29.09.2013 Initial version in git
                 03.09.2014 More generic datatype
 Description:    memory management for framebased data

 Implementation eines Speichermanagements für Frame basierende Daten
 wie z.b. CAN-Messages.

------------------------------------------------------------------------------*/

#include "config.h"

#ifndef _FRAMESTORAGE_H_
	#define _FRAMESTORAGE_H_

#if USE_FRAMESTORAGE

	#include <avr/io.h>
	#include <string.h>
	
	#include "can2udp.h"

	#define FS_DATA_SIZE 20

	#define FS_DATA_TYPE can_frame_t

	extern FS_DATA_TYPE framestorage_data[];


	void framestorage_init(void);
	void framestorage_item_clear(uint8_t unit_id);
	uint8_t framestorage_item_next(void);

#endif
#endif // _FRAMESTORAGE_H_


