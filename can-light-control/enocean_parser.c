/*----------------------------------------------------------------------------
 Copyright:      Ulrich Escher  mailto: code@myvdr.de
 Author:         Ulrich Escher
 Remarks:        
 known Problems: none
 Version:        13.01.2012
 Description:    Enocean Telegram Parser

 read enocean telegram stream and save in the enocean_parser_t struct

------------------------------------------------------------------------------*/

#include <avr/io.h>

#include "enocean_parser.h"


#define ENOCEAN_PARSER_SYNC1    0xA5
#define ENOCEAN_PARSER_SYNC2    0x5A

uint8_t enocean_parser_state = ENOCEAN_PARSER_STATE_SYNC1;

uint8_t enocean_parser_cnt_db;
uint8_t enocean_parser_cnt_id;
uint8_t enocean_parser_chk;

enocean_parser_t enocean_parser_input;



void enocean_parser_init()
{
	return;
}


uint8_t enocean_parser_poll(uint8_t tmp)
{

	if ( enocean_parser_state >= ENOCEAN_PARSER_STATE_TYPE && enocean_parser_state <= ENOCEAN_PARSER_STATE_STATUS )
		enocean_parser_chk += tmp;

	switch(enocean_parser_state)
	{
		case ENOCEAN_PARSER_STATE_SYNC1:
			if (tmp == ENOCEAN_PARSER_SYNC1)
			{
				enocean_parser_chk = 0;
				enocean_parser_state = ENOCEAN_PARSER_STATE_SYNC2;
			}
			break;
		case ENOCEAN_PARSER_STATE_SYNC2:
			if (tmp == ENOCEAN_PARSER_SYNC2)
			{
				enocean_parser_state = ENOCEAN_PARSER_STATE_TYPE;
			} else {
				enocean_parser_state = ENOCEAN_PARSER_STATE_SYNC1;
			}
			break;
		case ENOCEAN_PARSER_STATE_TYPE:
			enocean_parser_input.type = tmp;
			enocean_parser_state = ENOCEAN_PARSER_STATE_ORG;
			break;
		case ENOCEAN_PARSER_STATE_ORG:
			enocean_parser_input.org = tmp;
			enocean_parser_state = ENOCEAN_PARSER_STATE_DB;
			enocean_parser_cnt_db = 4;
			enocean_parser_cnt_id = 4;
			break;
		case ENOCEAN_PARSER_STATE_DB:
			if (enocean_parser_cnt_db != 0)
			{
				if (enocean_parser_cnt_db == 4)
					enocean_parser_input.db[3] = tmp;
				if (enocean_parser_cnt_db == 3)
					enocean_parser_input.db[2] = tmp;
				if (enocean_parser_cnt_db == 2)
					enocean_parser_input.db[1] = tmp;
				if (enocean_parser_cnt_db == 1)
					enocean_parser_input.db[0] = tmp;
				enocean_parser_cnt_db--;
				if (enocean_parser_cnt_db == 0)
					enocean_parser_state = ENOCEAN_PARSER_STATE_ID;
			}
			break;
		case ENOCEAN_PARSER_STATE_ID:
			if (enocean_parser_cnt_id != 0)
			{
				if (enocean_parser_cnt_id == 4)
					enocean_parser_input.id[3] = tmp;
				if (enocean_parser_cnt_id == 3)
					enocean_parser_input.id[2] = tmp;
				if (enocean_parser_cnt_id == 2)
					enocean_parser_input.id[1] = tmp;
				if (enocean_parser_cnt_id == 1)
					enocean_parser_input.id[0] = tmp;
				enocean_parser_cnt_id--;
				if (enocean_parser_cnt_id == 0)
					enocean_parser_state = ENOCEAN_PARSER_STATE_STATUS;
			}
			break;
		case ENOCEAN_PARSER_STATE_STATUS:
			enocean_parser_input.status = tmp;
			enocean_parser_state = ENOCEAN_PARSER_STATE_CHK;
			break;

		case ENOCEAN_PARSER_STATE_CHK:
			enocean_parser_state = ENOCEAN_PARSER_STATE_SYNC1;
			if (enocean_parser_chk == tmp) {
				return ENOCEAN_PARSER_STATE_CHKOK;
			}
			break;
		default:
			enocean_parser_state = ENOCEAN_PARSER_STATE_SYNC1;
			break;
	}

	return enocean_parser_state;
}


