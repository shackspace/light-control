/*----------------------------------------------------------------------------
 Copyright:      Ulrich Escher  mailto: code@myvdr.de
 Author:         Ulrich Escher
 Remarks:        
 known Problems: none
 Version:        23.05.2014
 Description:    Power Management Headerfile

------------------------------------------------------------------------------*/

#ifndef _POWER_MGT_H_
#define _POWER_MGT_H_

#include "config.h"
#if USE_POWER_MGT

	void power_mgt_init(void);
	void power_mgt_main(void);
	void power_mgt_tick(void);

#endif // USE_POWER_MGT
#endif /* _POWER_MGT_H_ */
