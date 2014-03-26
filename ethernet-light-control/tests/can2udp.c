#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "greatest.h"


#define _ENC28J60_H_
#define _AVR_EEPROM_H_ 1
#define _BV(a) ((1 << a))

#include "../can2udp.h"
#include "../can2udp.c"

uint16_t get_mem_unused (void){};
