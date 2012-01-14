/*----------------------------------------------------------------------------
 Copyright:      Ulrich Escher  mailto: code@myvdr.de
 Author:         Ulrich Escher
 Remarks:        
 known Problems: none
 Version:        13.01.2012
 Description:    Human Machine Interface Headerfile

------------------------------------------------------------------------------*/

#ifndef _HMI_H_
#define _HMI_H_

#include <avr/io.h>

void hmi_init(void);
void hmi_main(void);
void hmi_tick(void);

void led_set(uint8_t led, uint8_t status);

#endif /* _HMI_H_ */
