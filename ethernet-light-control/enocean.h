/*----------------------------------------------------------------------------
------------------------------------------------------------------------------*/
#include "config.h"

#ifdef USE_ENOCEAN

	#define ENOCEAN_DEBUG usart_write
	//#define ENOCEAN_DEBUG(...)

	#include <avr/io.h>
	#include <avr/pgmspace.h>
	#include <avr/eeprom.h>
	#include <string.h>
	#include <util/delay.h>
	
	#include "stack.h"
	#include "usart.h"
	#include "cmd.h"

	#define ENOCEAN_PORT_EEPROM_STORE    	50
	#define ENOCEAN_LICHT_EEPROM_STORE    	60

	void enocean_init(void);
	void enocean_netInit(void);
	void enocean_main(void);
	void enocean_get(unsigned char);
	void enocean_tick(void);
	void enocean_packet_send(uint8_t addr, uint8_t status);

	extern volatile uint8_t key_state;
	extern volatile uint8_t key_press;
 

#endif



