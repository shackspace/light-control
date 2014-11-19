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
uint8_t framestorage_state[FS_DATA_SIZE];


// ----------------------------------------------------------------------------
// initialization framestorage_init()
void framestorage_init(void) {
	uint8_t i;
	for (i=0;i<FS_DATA_SIZE;i++) {
		memset(&framestorage_data[i],0,sizeof(FS_DATA_TYPE));
		framestorage_state[i] = 0;
	}
	return;
}


// ----------------------------------------------------------------------------
// decrease frame usage-counter
uint8_t framestorage_get(uint8_t unit_id) {
	if (framestorage_state[unit_id] == 0)
		return 255;
	if (framestorage_state[unit_id] > 0)
		framestorage_state[unit_id]--;
	if (framestorage_state[unit_id] == 0)
		memset(&framestorage_data[unit_id],0,sizeof(FS_DATA_TYPE));

	return framestorage_state[unit_id];
}


// ----------------------------------------------------------------------------
// increase frame usage-counter
uint8_t framestorage_put(uint8_t unit_id) {
	framestorage_state[unit_id]++;

	return framestorage_state[unit_id];
}


// ----------------------------------------------------------------------------
// find first free item
uint8_t framestorage_item_next(void) {
	uint8_t i;
	for (i=0;i<FS_DATA_SIZE;i++) {
		if (FS_STATUS_CHECK)
			return i;
	}
	return 255;
}





#endif
