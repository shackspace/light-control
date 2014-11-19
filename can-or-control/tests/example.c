#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#include "greatest.h"

#define _ENC28J60_H_

#include "../config.h"
#include "../fifo.c"
#include "../enocean.c"
#include "../framestorage.c"
#include "../shackbus.c"
#include "../power_mgt.c"

/* Declare a local suite. */
SUITE(suite);


TEST fifo_case(void) {
	fifo_t mock_can_infifo;
	uint8_t inbuf[10];
	fifo_init (&mock_can_infifo,   inbuf, 10);
	ASSERT(fifo_get_count(&mock_can_infifo) == 0);
	fifo_put (&mock_can_infifo, 10);
	ASSERT(fifo_get_count(&mock_can_infifo) == 1);
	PASS();
}

TEST framestorage_case(void) {
	framestorage_init();
	ASSERT(framestorage_item_next() == 0);
	framestorage_put(0);

	int x;
	for (x = 1; x < FS_DATA_SIZE; x++) {
		int tmp = framestorage_item_next();
		framestorage_put(tmp);
	}

	ASSERT(framestorage_item_next() == 255);

	framestorage_get(5);
	ASSERT(framestorage_item_next() == 5);

	PASS();
}

TEST uart_case(void) {
	uart_init();
	ASSERT(fifo_get_count(&uart_outfifo) == 0);
	uart_putc('A');
	ASSERT(fifo_get_count(&uart_outfifo) == 1);
	fifo_get(&uart_outfifo);
	ASSERT(fifo_get_count(&uart_outfifo) == 0);
	PASS();
}

TEST enocean_case(void) {
	uart_init();
	enocean_init();
	ASSERT(fifo_get_count(&uart_outfifo) == 0);
	enocean_main();
	ASSERT(enocean_check_uart_output(140, 0) == true);
	ASSERT(enocean_check_uart_output(141, 0) == true);
	enocean_main();
	ASSERT(enocean_check_uart_output(142, 0) == true);
	ASSERT(enocean_check_uart_output(143, 0) == true);
	ASSERT(enocean_check_uart_output(0, 0) == false);
	ASSERT(fifo_get_count(&uart_outfifo) == 0);

	enocean_main();
	ASSERT(fifo_get_count(&uart_outfifo) == 0);

	enocean_state_set(0,1);
	enocean_main();

	ASSERT(enocean_check_uart_output(140, 1) == true);
	ASSERT(fifo_get_count(&uart_outfifo) == 0);

	enocean_main();
	ASSERT(fifo_get_count(&uart_outfifo) == 0);

	eeprom_write_byte( ENOCEAN_CHANNEL_EEPROM_STORE+0, 1);
	eeprom_write_byte( ENOCEAN_CHANNEL_EEPROM_STORE+1, 255);
	eeprom_write_byte( ENOCEAN_CHANNEL_EEPROM_STORE+3, 1);
	enocean_init();
	ASSERT(fifo_get_count(&uart_outfifo) == 0);
	enocean_main();
	ASSERT(enocean_check_uart_output(140, 1) == true);
	ASSERT(enocean_check_uart_output(141, 0) == true);
	enocean_main();
	ASSERT(enocean_check_uart_output(142, 0) == true);
	ASSERT(enocean_check_uart_output(143, 1) == true);
	ASSERT(fifo_get_count(&uart_outfifo) == 0);

	PASS();
}

TEST shackbus_mock_can_outfifo_case(void) {
	extern fifo_t mock_can_outfifo;
	extern fifo_t mock_can_infifo;
	shackbus_init();
	can_mock_init();
	ASSERT(fifo_get_count(&mock_can_outfifo) == 0);
	shackbus_main();
	ASSERT(fifo_get_count(&mock_can_outfifo) == 0);

	can_t send_msg_cmp;
	send_msg_cmp.id = 1337;  //Absender = 2   Empfänger = 1
	send_msg_cmp.flags.rtr = 1;
	send_msg_cmp.flags.extended = 0;
	send_msg_cmp.length  = 3;
	send_msg_cmp.data[0]=123;
	send_msg_cmp.data[1]=132;
	send_msg_cmp.data[2]=1;

	ASSERT(framestorage_item_next() == 0);

	memcpy(&framestorage_data[0], &send_msg_cmp, sizeof(FS_DATA_TYPE));
	framestorage_put(0);
	ASSERT(fifo_get_count(&can_outfifo) == 0);
	fifo_put (&can_outfifo, 0);
	ASSERT(fifo_get_count(&can_outfifo) == 1);
	shackbus_main();

	bool can_compare_sended(can_t msg);
	ASSERT(can_compare_sended(send_msg_cmp));

	ASSERT(fifo_get_count(&can_outfifo) == 0);

	ASSERT(fifo_get_count(&mock_can_outfifo) == 0);
	shackbus_main();
	ASSERT(fifo_get_count(&mock_can_outfifo) == 0);

	ASSERT(framestorage_get(0) == 255);
	ASSERT(framestorage_item_next() == 0);

	PASS();
}

TEST shackbus_case(void) {
	extern fifo_t mock_can_outfifo;
	extern fifo_t mock_can_infifo;
	shackbus_init();
	can_mock_init();
	ASSERT(fifo_get_count(&mock_can_outfifo) == 0);
	ASSERT(fifo_get_count(&mock_can_infifo) == 0);
	shackbus_main();
	ASSERT(fifo_get_count(&mock_can_outfifo) == 0);
	ASSERT(fifo_get_count(&mock_can_infifo) == 0);

	shackbus_send_msg(123,132);
	shackbus_main();

	can_t send_msg_cmp;
	send_msg_cmp.id = ((3L<<26)+(4L<<22)+(6L<<14)+(5L<<6)+11L);  //Absender = 2   Empfänger = 1
	send_msg_cmp.flags.rtr = 0;
	send_msg_cmp.flags.extended = 1;
	send_msg_cmp.length  = 3;
	send_msg_cmp.data[0]=1;
	send_msg_cmp.data[1]=123;
	send_msg_cmp.data[2]=132;

	bool can_compare_sended(can_t msg);
	ASSERT(can_compare_sended(send_msg_cmp));


	ASSERT(fifo_get_count(&mock_can_outfifo) == 0);
	ASSERT(fifo_get_count(&mock_can_infifo) == 0);


	uint8_t can_input_message(can_t *msg);
	can_input_message(&send_msg_cmp);
	ASSERT(can_check_message());
	ASSERT(fifo_get_count(&mock_can_outfifo) == 0);
	ASSERT(fifo_get_count(&mock_can_infifo) == 15);

	shackbus_main();

	ASSERT(fifo_get_count(&mock_can_outfifo) == 0);
	ASSERT(fifo_get_count(&mock_can_infifo) == 0);

	PASS();
}

TEST shackbus_ping_case(void) {
	extern fifo_t mock_can_outfifo;
	extern fifo_t mock_can_infifo;
	shackbus_init();
	can_mock_init();
	ASSERT(fifo_get_count(&mock_can_outfifo) == 0);
	ASSERT(fifo_get_count(&mock_can_infifo) == 0);
	shackbus_main();
	ASSERT(fifo_get_count(&mock_can_outfifo) == 0);
	ASSERT(fifo_get_count(&mock_can_infifo) == 0);

	can_t send_msg_cmp;
	send_msg_cmp.id = ((3L<<26)+(4L<<22)+(8L<<14)+(8L<<6)+10L);  //Absender = 2   Empfänger = 1
	send_msg_cmp.flags.rtr = 0;
	send_msg_cmp.flags.extended = 1;
	send_msg_cmp.length  = 1;
	send_msg_cmp.data[0]=10;
	send_msg_cmp.data[1]=123;
	send_msg_cmp.data[2]=132;

	uint8_t can_input_message(can_t *msg);
	can_input_message(&send_msg_cmp);
	ASSERT(can_check_message());
	ASSERT(fifo_get_count(&mock_can_outfifo) == 0);
	ASSERT(fifo_get_count(&mock_can_infifo) == 15);

	shackbus_main();
	shackbus_main();

	ASSERT(fifo_get_count(&mock_can_outfifo) == 15);
	ASSERT(fifo_get_count(&mock_can_infifo) == 0);


	send_msg_cmp.id = ((3L<<26)+(4L<<22)+(8L<<14)+(8L<<6)+10L);  //Absender = 2   Empfänger = 1
	send_msg_cmp.length = 2;  //Absender = 2   Empfänger = 1
	send_msg_cmp.data[0] = 11;
	bool can_compare_sended(can_t msg);
	ASSERT(can_compare_sended(send_msg_cmp));

	ASSERT(fifo_get_count(&mock_can_outfifo) == 0);
	ASSERT(fifo_get_count(&mock_can_infifo) == 0);


	PASS();
}


TEST shackbus_channel_case(uint8_t channel) {
	extern fifo_t mock_can_outfifo;
	extern fifo_t mock_can_infifo;
	uart_init();
	enocean_mock_init();
	enocean_init();
	ASSERT(fifo_get_count(&uart_outfifo) == 0);
	enocean_main();
	ASSERT(enocean_check_uart_output(140, 0) == true);
	ASSERT(enocean_check_uart_output(141, 0) == true);
	enocean_main();
	ASSERT(enocean_check_uart_output(142, 0) == true);
	ASSERT(enocean_check_uart_output(143, 0) == true);
	ASSERT(fifo_get_count(&uart_outfifo) == 0);


	shackbus_init();
	can_mock_init();
	ASSERT(fifo_get_count(&mock_can_outfifo) == 0);
	ASSERT(fifo_get_count(&mock_can_infifo) == 0);
	shackbus_main();
	ASSERT(fifo_get_count(&mock_can_outfifo) == 0);
	ASSERT(fifo_get_count(&mock_can_infifo) == 0);

	can_t send_msg_cmp;
	send_msg_cmp.id = ((3L<<26)+(4L<<22)+(5L<<14)+(6L<<6)+11L);  //prio vlan src dst prot
	send_msg_cmp.flags.rtr = 0;
	send_msg_cmp.flags.extended = 1;
	send_msg_cmp.length  = 3;
	send_msg_cmp.data[0]=1;
	send_msg_cmp.data[1]=140+channel;
	send_msg_cmp.data[2]=1;

	uint8_t can_input_message(can_t *msg);
	can_input_message(&send_msg_cmp);
	ASSERT(can_check_message());
	ASSERT(fifo_get_count(&mock_can_outfifo) == 0);
	ASSERT(fifo_get_count(&mock_can_infifo) == 15);

	shackbus_main();
	shackbus_main();
	shackbus_main();

	ASSERT(fifo_get_count(&mock_can_outfifo) == 30);
	ASSERT(fifo_get_count(&mock_can_infifo) == 0);


	send_msg_cmp.id = ((3L<<26)+(4L<<22)+(8L<<14)+(5L<<6)+11L);  //prio vlan src dst prot
	send_msg_cmp.length = 3;  //Absender = 2   Empfänger = 1
	send_msg_cmp.data[0] = 1;
	send_msg_cmp.data[1]=140+channel;
	send_msg_cmp.data[2]=1;
	bool can_compare_sended(can_t msg);
	ASSERT(can_compare_sended(send_msg_cmp));

	ASSERT(fifo_get_count(&mock_can_outfifo) == 15);
	ASSERT(fifo_get_count(&mock_can_infifo) == 0);


	send_msg_cmp.id = ((3L<<26)+(4L<<22)+(6L<<14)+(5L<<6)+11L);  //Absender = 2   Empfänger = 1
	send_msg_cmp.flags.rtr = 0;
	send_msg_cmp.flags.extended = 1;
	send_msg_cmp.length  = 3;
	send_msg_cmp.data[0]=1;
	send_msg_cmp.data[1]=140+channel;
	send_msg_cmp.data[2]=1;

	bool can_compare_sended(can_t msg);
	ASSERT(can_compare_sended(send_msg_cmp));


	ASSERT(fifo_get_count(&mock_can_outfifo) == 0);
	ASSERT(fifo_get_count(&mock_can_infifo) == 0);

	ASSERT( enocean_state_get(channel) == 1);

	ASSERT(fifo_get_count(&uart_outfifo) == 0);
	enocean_main();

	ASSERT(enocean_check_uart_output(140+channel, 1) == true);
	
	ASSERT(fifo_get_count(&uart_outfifo) == 0);
	enocean_main();
	ASSERT(fifo_get_count(&uart_outfifo) == 0);



	PASS();
}


void power_mgt_case_tick_main(int tick, int main);
void power_mgt_case_tick_main(int tick, int main)
{
	for (int i=0; i<tick; i++) power_mgt_tick();
	for (int i=0; i<main; i++) power_mgt_main();
}

TEST power_mgt_case(uint8_t channel, uint8_t old, uint8_t new, uint8_t future) {
	
	extern fifo_t can_outfifo;

	power_mgt_init();
	power_mgt_set_input_1(channel, new);	
	if (channel == 0) if (new) key_state = 4; else key_state = 0;
	if (channel==3) power_mgt_set_wait_off(channel, 120*60);

	enocean_state_set(channel,old);
	power_mgt_case_tick_main(0, 1*4);
	ASSERT( enocean_state_get(channel) == old);

	power_mgt_case_tick_main(4-channel, 2*4);
	ASSERT( enocean_state_get(channel) == old);

	power_mgt_case_tick_main(1, 3*4);
	if (channel==3) power_mgt_case_tick_main(120*60, 2*4);
	ASSERT_EQ( enocean_state_get(channel), new);

	power_mgt_set_input_1(channel, future);	
	if (channel == 0) if (future) key_state = 4; else key_state = 0;

	power_mgt_case_tick_main(0, 2*4);
	if (channel==3) power_mgt_case_tick_main(120*60, 2*4);
	ASSERT_EQ( enocean_state_get(channel), future);

	PASS();
}


SUITE(suite) {
	RUN_TEST(fifo_case);
	RUN_TEST(framestorage_case);
	RUN_TEST(uart_case);
	RUN_TEST(enocean_case);
	RUN_TEST(shackbus_mock_can_outfifo_case);
	RUN_TEST(shackbus_case);
	RUN_TEST(shackbus_ping_case);
	RUN_TESTp(shackbus_channel_case,0);
	RUN_TESTp(shackbus_channel_case,1);
	RUN_TESTp(shackbus_channel_case,2);
	RUN_TESTp(shackbus_channel_case,3);
	RUN_TESTp(power_mgt_case, 0, 0, 0, 0);
	RUN_TESTp(power_mgt_case, 0, 0, 0, 1);
	RUN_TESTp(power_mgt_case, 0, 0, 1, 0);
	RUN_TESTp(power_mgt_case, 0, 0, 1, 1);
	RUN_TESTp(power_mgt_case, 0, 1, 0, 0);
	RUN_TESTp(power_mgt_case, 0, 1, 0, 1);
	RUN_TESTp(power_mgt_case, 0, 1, 1, 0);
	RUN_TESTp(power_mgt_case, 0, 1, 1, 1);

	RUN_TESTp(power_mgt_case, 1, 0, 0, 0);
	RUN_TESTp(power_mgt_case, 1, 0, 0, 1);
	RUN_TESTp(power_mgt_case, 1, 0, 1, 0);
	RUN_TESTp(power_mgt_case, 1, 0, 1, 1);
	RUN_TESTp(power_mgt_case, 1, 1, 0, 0);
	RUN_TESTp(power_mgt_case, 1, 1, 0, 1);
	RUN_TESTp(power_mgt_case, 1, 1, 1, 0);
	RUN_TESTp(power_mgt_case, 1, 1, 1, 1);

	RUN_TESTp(power_mgt_case, 2, 0, 0, 0);
	RUN_TESTp(power_mgt_case, 2, 0, 0, 1);
	RUN_TESTp(power_mgt_case, 2, 0, 1, 0);
	RUN_TESTp(power_mgt_case, 2, 0, 1, 1);
	RUN_TESTp(power_mgt_case, 2, 1, 0, 0);
	RUN_TESTp(power_mgt_case, 2, 1, 0, 1);
	RUN_TESTp(power_mgt_case, 2, 1, 1, 0);
	RUN_TESTp(power_mgt_case, 2, 1, 1, 1);

	RUN_TESTp(power_mgt_case, 3, 0, 0, 0);
	RUN_TESTp(power_mgt_case, 3, 0, 0, 1);
	RUN_TESTp(power_mgt_case, 3, 0, 1, 0);
	RUN_TESTp(power_mgt_case, 3, 0, 1, 1);
	RUN_TESTp(power_mgt_case, 3, 1, 0, 0);
	RUN_TESTp(power_mgt_case, 3, 1, 0, 1);
	RUN_TESTp(power_mgt_case, 3, 1, 1, 0);
	RUN_TESTp(power_mgt_case, 3, 1, 1, 1);

}

/* Add all the definitions that need to be in the test runner's main file. */
GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();      /* command-line arguments, initialization. */
    RUN_SUITE(suite);
    GREATEST_MAIN_END();        /* display results */
}
