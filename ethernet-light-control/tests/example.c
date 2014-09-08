#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "greatest.h"

#define _ENC28J60_H_

#include "../config.h"
#include "../fifo.c"
#include "../enocean.c"
#include "../framestorage.c"
#include "../shackbus.c"
#include "../stack.h"

/* Declare a local suite. */
SUITE(suite);


TEST fifo_case(void) {
	fifo_t can_infifo;
	uint8_t inbuf[10];
	fifo_init (&can_infifo,   inbuf, 10);
	ASSERT(fifo_get_count(&can_infifo) == 0);
	fifo_put (&can_infifo, 10);
	ASSERT(fifo_get_count(&can_infifo) == 1);
	PASS();
}

TEST framestorage_case(void) {
	framestorage_init();
	ASSERT(framestorage_item_next() == 0);
	framestorage_data[0].state = 1;

	int x;
	for (x = 1; x < FS_DATA_SIZE; x++) {
		int tmp = framestorage_item_next();
		framestorage_data[tmp].state = 1;
	}

	ASSERT(framestorage_item_next() == 255);

	framestorage_item_clear(5);
	ASSERT(framestorage_item_next() == 5);

	PASS();
}

TEST send_udp_msg_case(void) {
	extern fifo_t eth_outfifo;	
	void init_eth_mock(void);
	void clean_eth_buffer(void);

	init_eth_mock();
	clean_eth_buffer();
	send_udp_msg(0,1);
	ASSERT(fifo_get_count(&eth_outfifo) == 16);
	ASSERT(fifo_get(&eth_outfifo) == 20);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);


	init_eth_mock();
	clean_eth_buffer();
	send_udp_msg(1,1);
	ASSERT(fifo_get_count(&eth_outfifo) == 16);
	ASSERT(fifo_get(&eth_outfifo) == 21);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);


	init_eth_mock();
	clean_eth_buffer();
	send_udp_msg(2,1);
	ASSERT(fifo_get_count(&eth_outfifo) == 16);
	ASSERT(fifo_get(&eth_outfifo) == 22);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);


	init_eth_mock();
	clean_eth_buffer();
	send_udp_msg(3,1);
	ASSERT(fifo_get_count(&eth_outfifo) == 16);
	ASSERT(fifo_get(&eth_outfifo) == 23);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);


	init_eth_mock();
	clean_eth_buffer();
	send_udp_msg(4,1);
	ASSERT(fifo_get_count(&eth_outfifo) == 16);
	ASSERT(fifo_get(&eth_outfifo) == 24);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);
	ASSERT(fifo_get(&eth_outfifo) == 2);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);


	init_eth_mock();
	clean_eth_buffer();
	send_udp_msg(5,1);
	ASSERT(fifo_get_count(&eth_outfifo) == 16);
	ASSERT(fifo_get(&eth_outfifo) == 25);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);
	ASSERT(fifo_get(&eth_outfifo) == 3);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);


	init_eth_mock();
	clean_eth_buffer();
	send_udp_msg(6,1);
	ASSERT(fifo_get_count(&eth_outfifo) == 16);
	ASSERT(fifo_get(&eth_outfifo) == 26);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);
	ASSERT(fifo_get(&eth_outfifo) == 4);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);


	init_eth_mock();
	clean_eth_buffer();
	send_udp_msg(7,1);
	ASSERT(fifo_get_count(&eth_outfifo) == 16);
	ASSERT(fifo_get(&eth_outfifo) == 27);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);
	ASSERT(fifo_get(&eth_outfifo) == 4);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);


	init_eth_mock();
	clean_eth_buffer();
	send_udp_msg(8,1);
	ASSERT(fifo_get_count(&eth_outfifo) == 16);
	ASSERT(fifo_get(&eth_outfifo) == 28);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);
	ASSERT(fifo_get(&eth_outfifo) == 5);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);


	init_eth_mock();
	clean_eth_buffer();
	send_udp_msg(9,1);
	ASSERT(fifo_get_count(&eth_outfifo) == 16);
	ASSERT(fifo_get(&eth_outfifo) == 29);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);
	ASSERT(fifo_get(&eth_outfifo) == 6);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);


	init_eth_mock();
	clean_eth_buffer();
	send_udp_msg(10,1);
	ASSERT(fifo_get_count(&eth_outfifo) == 16);
	ASSERT(fifo_get(&eth_outfifo) == 30);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);
	ASSERT(fifo_get(&eth_outfifo) == 7);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);


	init_eth_mock();
	clean_eth_buffer();
	send_udp_msg(11,1);
	ASSERT(fifo_get_count(&eth_outfifo) == 8);
	ASSERT(fifo_get(&eth_outfifo) == 31);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);


	init_eth_mock();
	clean_eth_buffer();
	send_udp_msg(12,1);
	ASSERT(fifo_get_count(&eth_outfifo) == 0);


	init_eth_mock();
	clean_eth_buffer();
	send_udp_msg(120,1);
	ASSERT(fifo_get_count(&eth_outfifo) == 12);
	ASSERT(fifo_get(&eth_outfifo) == 10);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);
	fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);fifo_get(&eth_outfifo);



	PASS();
}


SUITE(suite) {
	RUN_TEST(fifo_case);
	RUN_TEST(framestorage_case);
	RUN_TEST(send_udp_msg_case);

}

/* Add all the definitions that need to be in the test runner's main file. */
GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();      /* command-line arguments, initialization. */
    RUN_SUITE(suite);
    GREATEST_MAIN_END();        /* display results */
}
