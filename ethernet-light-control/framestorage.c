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

#include "framestorage.h"

#if USE_FRAMESTORAGE

#include <stdio.h>



FS_DATA_TYPE framestorage_data[FS_DATA_SIZE];


// ----------------------------------------------------------------------------
// initialization framestorage_init()
void framestorage_init(void) {
	uint8_t i;
	for (i=0;i<FS_DATA_SIZE;i++) {
		framestorage_item_clear(i);
	}
	return;
}


// ----------------------------------------------------------------------------
// clear item
void framestorage_item_clear(uint8_t unit_id) {
	memset(&framestorage_data[unit_id],0,sizeof(FS_DATA_TYPE));
	return;
}


// ----------------------------------------------------------------------------
// find first free item
uint8_t framestorage_item_next(void) {
	uint8_t i;
	for (i=0;i<FS_DATA_SIZE;i++) {
		if (framestorage_data[i].state == 0)
			return i;
	}
	return 255;
}





#endif
