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

	#define POWER_MGT_CHANNEL_COUNT           4

	void power_mgt_init(void);
	void power_mgt_set_input_1(uint8_t _channel, uint8_t _state);
	void power_mgt_set_wait_on(uint8_t _channel, uint16_t _value);
	void power_mgt_set_wait_off(uint8_t _channel, uint16_t _value);
	void power_mgt_main(void);
	void power_mgt_tick(void);
	void power_mgt_msg_send(void);

#endif // USE_POWER_MGT
#endif /* _POWER_MGT_H_ */
