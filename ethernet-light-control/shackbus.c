/*----------------------------------------------------------------------------
 Copyright:      Ulrich Escher  mailto: code@myvdr.de
 Author:         Ulrich Escher
 Remarks:        
 known Problems: none
 Version:        11.04.2012
 Description:    shackbus Interface


------------------------------------------------------------------------------*/

#include "config.h"
#if USE_SHACKBUS

#include <avr/io.h>
#include "shackbus.h"

#include "can.h"



uint8_t shackbus_id2sb(shackbus_id_t* shackbus, can_t msg)
{

  shackbus->prio = (msg.id >> 26) & 0x07;
  shackbus->vlan = (msg.id >> 22) & 0x0F;
  shackbus->src  = (msg.id >> 14) & 0xFF;
  shackbus->dst  = (msg.id >> 06) & 0xFF;
  shackbus->prot = (msg.id >> 00) & 0x3F;

	return 0;
}

uint32_t shackbus_sb2id(shackbus_id_t* sb)
{
    sb->prio &= 0x07;
    sb->vlan &= 0x0F;
    sb->src  &= 0xFF;
    sb->dst  &= 0xFF;
    sb->prot &= 0x3F;

    return (uint32_t)((uint32_t)0x80000000+((uint32_t)sb->prio<<26)+((uint32_t)sb->vlan<<22)+((uint32_t)sb->src<<14)+((uint32_t)sb->dst<<6)+(uint32_t)sb->prot);
}

#endif // USE_SHACKBUS
