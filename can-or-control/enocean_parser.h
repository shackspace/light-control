/*----------------------------------------------------------------------------
 Copyright:      Ulrich Escher  mailto: code@myvdr.de
 Author:         Ulrich Escher
 Remarks:        
 known Problems: none
 Version:        13.01.2012
 Description:    Enocean Telegram Parser Headerfile

 read enocean telegram stream and save in the enocean_parser_t struct

------------------------------------------------------------------------------*/

#ifndef _ENOCEAN_PARSER_H_
#define _ENOCEAN_PARSER_H_

#include "config.h"

#if USE_ENOCEAN_PARSER


#include <avr/io.h>
#include <util/crc16.h>

void enocean_parser_init(void);
uint8_t enocean_parser_poll(uint8_t);

typedef struct { 
	uint8_t type;
	uint8_t org;
	uint8_t db[4];
	uint8_t id[4];
	uint8_t status;	
} enocean_parser_t;

extern enocean_parser_t enocean_parser_input;

#define ENOCEAN_PARSER_STATE_SYNC1   0
#define ENOCEAN_PARSER_STATE_SYNC2   1
#define ENOCEAN_PARSER_STATE_TYPE    2
#define ENOCEAN_PARSER_STATE_ORG     3
#define ENOCEAN_PARSER_STATE_DB      4
#define ENOCEAN_PARSER_STATE_ID      5
#define ENOCEAN_PARSER_STATE_STATUS  6
#define ENOCEAN_PARSER_STATE_CHK     7
#define ENOCEAN_PARSER_STATE_CHKOK   8


#endif //_enocean_parser_H_
#endif //USE_ENOCEAN_PARSER
