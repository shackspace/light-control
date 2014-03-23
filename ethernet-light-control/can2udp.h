/*----------------------------------------------------------------------------
------------------------------------------------------------------------------*/
#include "config.h"

#ifdef USE_CAN2UDP

#ifndef _CAN2UDP_H_
	#define _CAN2UDP_H_	

	#define CAN2UDP_DEBUG usart_write
	//#define CAN2UDP_DEBUG(...)

	#include <avr/io.h>
	#include <avr/pgmspace.h>
	#include <avr/eeprom.h>
	#include <string.h>
	#include <util/delay.h>
	
	#include "stack.h"
	#include "usart.h"
	#include "cmd.h"

	#include "can.h"

	void can2udp_init(void);
	void can2udp_netInit(void);
	void can2udp_main(void);
	void can2udp_get(unsigned char);
	void can2udp_tick(void);
	void can2udp_tick_10ms(void);
	void can2udp_packet_send(uint8_t addr, uint8_t status);
	void can2udp(can_t *msg);
	void can_t_2_can_frame_t(can_t* a, can_frame_t* b);
	void can_frame_t_2_can_t(can_frame_t* a, can_t* b);

	typedef struct
	{
		uint32_t can_id; //32 bit CAN_ID + EFF/RTR/ERR flags
		uint8_t can_dlc; //data length code
		uint8_t state;   // 0=>Data available 1=>Ack 2=>Syn (3=>Fin)?? 4=>Reset
		uint8_t tmp[2];
		uint8_t data[8];  //data
	} can_frame_t;

	/* special address description flags for the CAN_ID */
	#define CAN_EFF_FLAG 0x80000000U /* EFF/SFF is set in the MSB */
	#define CAN_RTR_FLAG 0x40000000U /* remote transmission request */
	#define CAN_ERR_FLAG 0x20000000U /* error frame */


#endif
#endif // _CAN2UDP_H_


