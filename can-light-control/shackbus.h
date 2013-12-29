// coding: utf-8
// ----------------------------------------------------------------------------
/*




 */
// ----------------------------------------------------------------------------
/**
 * \file    shackbus.h
 * \brief   Header-Datei für das allgemeine CAN Interface für Shackbus
 */
// ----------------------------------------------------------------------------

#ifndef SHACKBUS_H
#define SHACKBUS_H

// ----------------------------------------------------------------------------

#include <avr/pgmspace.h>
#include <stdint.h>
#include <stdbool.h>

#include "config.h"
#include "hmi.h"
#include "enocean.h"

void shackbus_init(void);
void shackbus_main(void);

uint8_t shackbus_send_msg(uint8_t val1, uint8_t val2);

typedef struct
{
	uint8_t prio; //Priorität 3bit
	uint8_t vlan; //VLAN 4bit
	uint8_t src;  //SRC 8bit
	uint8_t dst;  //DST 8bit
	uint8_t prot; //prot 6bit
} shackbus_id_t;

/*
Protokolle:
Register Control:
  prot = 10;
  len = 2byte
  db0 = mode
    0 = nothing
    1 = LED
    2 = ACT
    4 = EE
  db1 = channel
  db2 = state
  
*/


#endif // CAN_H
