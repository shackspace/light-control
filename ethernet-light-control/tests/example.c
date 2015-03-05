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
	ASSERT(fifo_get_count(&eth_outfifo) == 8);
	ASSERT(fifo_get(&eth_outfifo) == 20);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	get_eth_mock(2);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	get_eth_mock(2);


	clean_eth_buffer();
	send_udp_msg(1,1);
	ASSERT(fifo_get_count(&eth_outfifo) == 8);
	ASSERT(fifo_get(&eth_outfifo) == 21);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	get_eth_mock(2);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	get_eth_mock(2);


	clean_eth_buffer();
	send_udp_msg(2,1);
	ASSERT(fifo_get_count(&eth_outfifo) == 8);
	ASSERT(fifo_get(&eth_outfifo) == 22);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	get_eth_mock(2);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	get_eth_mock(2);


	clean_eth_buffer();
	send_udp_msg(3,1);
	ASSERT(fifo_get_count(&eth_outfifo) == 8);
	ASSERT(fifo_get(&eth_outfifo) == 23);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	get_eth_mock(2);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	get_eth_mock(2);


	clean_eth_buffer();
	send_udp_msg(4,1);
	ASSERT(fifo_get_count(&eth_outfifo) == 8);
	ASSERT(fifo_get(&eth_outfifo) == 24);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	get_eth_mock(2);
	ASSERT(fifo_get(&eth_outfifo) == 2);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	get_eth_mock(2);


	clean_eth_buffer();
	send_udp_msg(5,1);
	ASSERT(fifo_get_count(&eth_outfifo) == 8);
	ASSERT(fifo_get(&eth_outfifo) == 25);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	get_eth_mock(2);
	ASSERT(fifo_get(&eth_outfifo) == 3);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	get_eth_mock(2);


	clean_eth_buffer();
	send_udp_msg(6,1);
	ASSERT(fifo_get_count(&eth_outfifo) == 8);
	ASSERT(fifo_get(&eth_outfifo) == 26);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	get_eth_mock(2);
	ASSERT(fifo_get(&eth_outfifo) == 4);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	get_eth_mock(2);


	clean_eth_buffer();
	send_udp_msg(7,1);
	ASSERT(fifo_get_count(&eth_outfifo) == 8);
	ASSERT(fifo_get(&eth_outfifo) == 27);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	get_eth_mock(2);
	ASSERT(fifo_get(&eth_outfifo) == 4);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	get_eth_mock(2);


	clean_eth_buffer();
	send_udp_msg(8,1);
	ASSERT(fifo_get_count(&eth_outfifo) == 8);
	ASSERT(fifo_get(&eth_outfifo) == 28);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	get_eth_mock(2);
	ASSERT(fifo_get(&eth_outfifo) == 5);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	get_eth_mock(2);


	clean_eth_buffer();
	send_udp_msg(9,1);
	ASSERT(fifo_get_count(&eth_outfifo) == 8);
	ASSERT(fifo_get(&eth_outfifo) == 29);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	get_eth_mock(2);
	ASSERT(fifo_get(&eth_outfifo) == 6);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	get_eth_mock(2);


	clean_eth_buffer();
	send_udp_msg(10,1);
	ASSERT(fifo_get_count(&eth_outfifo) == 8);
	ASSERT(fifo_get(&eth_outfifo) == 30);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	get_eth_mock(2);
	ASSERT(fifo_get(&eth_outfifo) == 7);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	get_eth_mock(2);


	clean_eth_buffer();
	send_udp_msg(11,1);
	ASSERT(fifo_get_count(&eth_outfifo) == 4);
	ASSERT(fifo_get(&eth_outfifo) == 31);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	get_eth_mock(2);


	clean_eth_buffer();
	send_udp_msg(12,1);
	ASSERT(fifo_get_count(&eth_outfifo) == 0);


	clean_eth_buffer();
	send_udp_msg(120,1);
	ASSERT(fifo_get_count(&eth_outfifo) == 6);
	ASSERT(fifo_get(&eth_outfifo) == 10);
	ASSERT(fifo_get(&eth_outfifo) == 1);
	get_eth_mock(4);



	PASS();
}


TEST send_can2udp_case(void) {
	extern fifo_t eth_outfifo;	
	void init_eth_mock(void);
	void clean_eth_buffer(void);

	init_eth_mock();
	clean_eth_buffer();

	ASSERT(fifo_get_count(&eth_outfifo) == 0);

	extern fifo_t can2udp_infifo;
	extern fifo_t can2udp_outfifo;
	framestorage_init();

	can_t send_msg_cmp;
	send_msg_cmp.id = ((10L<<24)+(20L<<16)+(30L<<8)+40L);  //Absender = 2   Empfänger = 1
	send_msg_cmp.flags.rtr = 0;
	send_msg_cmp.flags.extended = 1;
	send_msg_cmp.length  = 3;
	send_msg_cmp.data[0]=10;
	send_msg_cmp.data[1]=123;
	send_msg_cmp.data[2]=132;

	can2udp_init();
	ASSERT(fifo_get_count(&can2udp_infifo) == 0);
	ASSERT(fifo_get_count(&can2udp_outfifo) == 0);
	can2udp(&send_msg_cmp);
	send_msg_cmp.data[0]=100;
	can2udp(&send_msg_cmp);
	ASSERT(fifo_get_count(&can2udp_infifo) == 0);
	ASSERT(fifo_get_count(&can2udp_outfifo) == 2);

	extern uint8_t c2u_con_state;
	c2u_con_state = 2;
	extern uint8_t c2u_con_state_tx;
	c2u_con_state_tx = 2;
	extern long c2u_remote_ip;
	c2u_remote_ip = 1;
	can2udp_main();
	ASSERT(fifo_get_count(&can2udp_infifo) == 0);
	ASSERT(fifo_get_count(&can2udp_outfifo) == 1);


	ASSERT(fifo_get_count(&eth_outfifo) == 16);
	ASSERT(fifo_get(&eth_outfifo) == 40);
	ASSERT(fifo_get(&eth_outfifo) == 30);
	ASSERT(fifo_get(&eth_outfifo) == 20);
	ASSERT(fifo_get(&eth_outfifo) == 10);
	get_eth_mock(4);
	ASSERT(fifo_get(&eth_outfifo) == 10);
	ASSERT(fifo_get(&eth_outfifo) == 123);
	ASSERT(fifo_get(&eth_outfifo) == 132);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	get_eth_mock(4);

	ASSERT(fifo_get_count(&can2udp_infifo) == 0);
	ASSERT(fifo_get_count(&can2udp_outfifo) == 1);
	ASSERT(fifo_get_count(&eth_outfifo) == 0);



	can2udp_main();

	send_msg_cmp.data[0]=200;
	can2udp(&send_msg_cmp);

	ASSERT(fifo_get_count(&eth_outfifo) == 16);
	ASSERT(fifo_get(&eth_outfifo) == 40);
	ASSERT(fifo_get(&eth_outfifo) == 30);
	ASSERT(fifo_get(&eth_outfifo) == 20);
	ASSERT(fifo_get(&eth_outfifo) == 10);
	get_eth_mock(4);
	ASSERT(fifo_get(&eth_outfifo) == 100);
	ASSERT(fifo_get(&eth_outfifo) == 123);
	ASSERT(fifo_get(&eth_outfifo) == 132);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	get_eth_mock(4);

	ASSERT(fifo_get_count(&can2udp_infifo) == 0);
	ASSERT(fifo_get_count(&can2udp_outfifo) == 1);
	ASSERT(fifo_get_count(&eth_outfifo) == 0);

	can2udp_main();

	ASSERT(fifo_get_count(&eth_outfifo) == 16);
	ASSERT(fifo_get(&eth_outfifo) == 40);
	ASSERT(fifo_get(&eth_outfifo) == 30);
	ASSERT(fifo_get(&eth_outfifo) == 20);
	ASSERT(fifo_get(&eth_outfifo) == 10);
	get_eth_mock(4);
	ASSERT(fifo_get(&eth_outfifo) == 200);
	ASSERT(fifo_get(&eth_outfifo) == 123);
	ASSERT(fifo_get(&eth_outfifo) == 132);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	get_eth_mock(4);

	ASSERT(fifo_get_count(&can2udp_infifo) == 0);
	ASSERT(fifo_get_count(&can2udp_outfifo) == 0);
	ASSERT(fifo_get_count(&eth_outfifo) == 0);



	PASS();
}


TEST recv_can2udp_case(void) {
	extern fifo_t eth_outfifo;
	void init_eth_mock(void);
	void clean_eth_buffer(void);

	init_eth_mock();
	clean_eth_buffer();

	ASSERT(fifo_get_count(&eth_outfifo) == 0);

	extern fifo_t can2udp_infifo;
	extern fifo_t can2udp_outfifo;
	framestorage_init();

	extern uint8_t c2u_con_state;
	c2u_con_state = 2;
	extern uint8_t c2u_con_state_tx;
	c2u_con_state_tx = 2;
	extern long c2u_remote_ip;
	c2u_remote_ip = 1;

	extern uint8_t lost_can_frames3;
	extern uint8_t c2u_state;



	can_t send_msg;
	send_msg.id = ((10L<<24)+(20L<<16)+(30L<<8)+40L);  //Absender = 2   Empfänger = 1
	send_msg.flags.rtr = 0;
	send_msg.flags.extended = 1;
	send_msg.length  = 3;
	send_msg.data[0]=10;
	send_msg.data[1]=123;
	send_msg.data[2]=132;

	can2udp_init();


	ASSERT(fifo_get_count(&can2udp_infifo) == 0);
	ASSERT(fifo_get_count(&can2udp_outfifo) == 0);
	ASSERT(fifo_get_count(&eth_outfifo) == 0);


	can2udp_main();

	ASSERT(fifo_get_count(&can2udp_infifo) == 0);
	ASSERT(fifo_get_count(&can2udp_outfifo) == 0);
	ASSERT(fifo_get_count(&eth_outfifo) == 0);




	extern fifo_t mock_can_outfifo;
	extern fifo_t mock_can_infifo;
	void can_mock_init(void);
	shackbus_init();
	can_mock_init();
	ASSERT(fifo_get_count(&mock_can_outfifo) == 0);
	ASSERT(fifo_get_count(&mock_can_infifo) == 0);
	shackbus_main();
	ASSERT(fifo_get_count(&mock_can_outfifo) == 0);
	ASSERT(fifo_get_count(&mock_can_infifo) == 0);




	eth_buffer[IP_PKTLEN]   = 0;
	eth_buffer[IP_PKTLEN+1] = 16 + 28;
	ASSERT( (UDP_DATA_START) == 42);
	ASSERT( (UDP_DATA_END_VAR - UDP_DATA_START) == 16);

	eth_buffer[UDP_DATA_START+5] = 1; //State
	ASSERT(lost_can_frames3 == 0);
	ASSERT(c2u_state == 0);
	can2udp_get(0);
	ASSERT(lost_can_frames3 == 0);
	ASSERT(c2u_state == 1);


	ASSERT(c2u_con_state_tx == 2);

	ASSERT(fifo_get_count(&can2udp_infifo) == 1);
	ASSERT(fifo_get_count(&can2udp_outfifo) == 1);
	ASSERT(fifo_get_count(&eth_outfifo) == 0);

	can2udp_main();
	shackbus_main();

	ASSERT(fifo_get_count(&eth_outfifo) == 16);

	ASSERT(fifo_get(&eth_outfifo) == 2);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	ASSERT(fifo_get(&eth_outfifo) == 0xA0);
	get_eth_mock(12);
	ASSERT(fifo_get_count(&can2udp_infifo) == 0);
	ASSERT(fifo_get_count(&can2udp_outfifo) == 0);
	ASSERT(fifo_get_count(&eth_outfifo) == 0);



	can_t send_msg_cmp;
	send_msg_cmp.id = 0;//((10L<<24)+(20L<<16)+(30L<<8)+40L);  //Absender = 2   Empfänger = 1
	send_msg_cmp.flags.rtr = 0;
	send_msg_cmp.flags.extended = 0;
	send_msg_cmp.length  = 0;
	send_msg_cmp.data[0]=0;
	send_msg_cmp.data[1]=0;
	send_msg_cmp.data[2]=0;

	bool can_compare_sended(can_t msg);
	can_compare_sended(send_msg_cmp);

	ASSERT(fifo_get_count(&mock_can_outfifo) == 0);
	ASSERT(fifo_get_count(&mock_can_infifo) == 0);


	eth_buffer[IP_PKTLEN]   = 0;
	eth_buffer[IP_PKTLEN+1] = 16 + 28;
	ASSERT( (UDP_DATA_START) == 42);
	ASSERT( (UDP_DATA_END_VAR - UDP_DATA_START) == 16);

	eth_buffer[UDP_DATA_START+0] = 0; //ID
	eth_buffer[UDP_DATA_START+1] = 0;
	eth_buffer[UDP_DATA_START+2] = 0;
	eth_buffer[UDP_DATA_START+3] = 0;

	eth_buffer[UDP_DATA_START+4] = 0; //LEN
	eth_buffer[UDP_DATA_START+5] = 1; //State
	eth_buffer[UDP_DATA_START+6] = 1; //SEQ
	eth_buffer[UDP_DATA_START+7] = 0;
	ASSERT(lost_can_frames3 == 0);
	ASSERT(c2u_state == 1);
	can2udp_get(0);
	ASSERT(lost_can_frames3 == 0);
	ASSERT(c2u_state == 1);



	ASSERT(fifo_get_count(&can2udp_infifo) == 1);
	ASSERT(fifo_get_count(&can2udp_outfifo) == 0);
	send_msg.data[0]=100;
	can2udp(&send_msg);
	ASSERT(fifo_get_count(&can2udp_infifo) == 1);
	ASSERT(fifo_get_count(&can2udp_outfifo) == 1);


	ASSERT(c2u_con_state_tx == 3);
	c2u_con_state_tx = 2;
	can2udp_main();
	ASSERT(c2u_con_state_tx == 2);



	can2udp_main();
	can2udp_main();
	shackbus_main();
	ASSERT(fifo_get_count(&can2udp_infifo) == 0);
	ASSERT(fifo_get_count(&can2udp_outfifo) == 0);

	ASSERT(can_compare_sended(send_msg_cmp));

	ASSERT(fifo_get_count(&mock_can_outfifo) == 0);
	ASSERT(fifo_get_count(&mock_can_infifo) == 0);



	ASSERT(fifo_get_count(&eth_outfifo) == 16);
	ASSERT(fifo_get_count(&can2udp_infifo) == 0);
	ASSERT(fifo_get_count(&can2udp_outfifo) == 0);
	can2udp_main();
	ASSERT(fifo_get_count(&eth_outfifo) == 16);
	ASSERT(fifo_get_count(&can2udp_infifo) == 0);
	ASSERT(fifo_get_count(&can2udp_outfifo) == 0);
	can2udp_main();
	ASSERT(fifo_get_count(&eth_outfifo) == 16);
	ASSERT(fifo_get_count(&can2udp_infifo) == 0);
	ASSERT(fifo_get_count(&can2udp_outfifo) == 0);

	send_msg.data[0]=200;
	can2udp(&send_msg);
	send_msg.data[0]=210;
	can2udp(&send_msg);

	ASSERT(fifo_get_count(&eth_outfifo) == 16);
	ASSERT(fifo_get(&eth_outfifo) == 40);
	ASSERT(fifo_get(&eth_outfifo) == 30);
	ASSERT(fifo_get(&eth_outfifo) == 20);
	ASSERT(fifo_get(&eth_outfifo) == 10);
	get_eth_mock(4);
	ASSERT(fifo_get(&eth_outfifo) == 100);
	ASSERT(fifo_get(&eth_outfifo) == 123);
	ASSERT(fifo_get(&eth_outfifo) == 132);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	get_eth_mock(4);

	ASSERT(fifo_get_count(&can2udp_infifo) == 0);
	ASSERT(fifo_get_count(&can2udp_outfifo) == 2);
	ASSERT(fifo_get_count(&eth_outfifo) == 0);

	can2udp_main();
	ASSERT(fifo_get_count(&can2udp_infifo) == 0);
	ASSERT(fifo_get_count(&can2udp_outfifo) == 1);

	ASSERT(fifo_get_count(&eth_outfifo) == 16);
	ASSERT(fifo_get(&eth_outfifo) == 40);
	ASSERT(fifo_get(&eth_outfifo) == 30);
	ASSERT(fifo_get(&eth_outfifo) == 20);
	ASSERT(fifo_get(&eth_outfifo) == 10);
	get_eth_mock(4);
	ASSERT(fifo_get(&eth_outfifo) == 200);
	ASSERT(fifo_get(&eth_outfifo) == 123);
	ASSERT(fifo_get(&eth_outfifo) == 132);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	get_eth_mock(4);

	ASSERT(fifo_get_count(&can2udp_infifo) == 0);
	ASSERT(fifo_get_count(&can2udp_outfifo) == 1);
	ASSERT(fifo_get_count(&eth_outfifo) == 0);

	can2udp_main();
	ASSERT(fifo_get_count(&can2udp_infifo) == 0);
	ASSERT(fifo_get_count(&can2udp_outfifo) == 0);

	ASSERT(fifo_get_count(&eth_outfifo) == 16);
	ASSERT(fifo_get(&eth_outfifo) == 40);
	ASSERT(fifo_get(&eth_outfifo) == 30);
	ASSERT(fifo_get(&eth_outfifo) == 20);
	ASSERT(fifo_get(&eth_outfifo) == 10);
	get_eth_mock(4);
	ASSERT(fifo_get(&eth_outfifo) == 210);
	ASSERT(fifo_get(&eth_outfifo) == 123);
	ASSERT(fifo_get(&eth_outfifo) == 132);
	ASSERT(fifo_get(&eth_outfifo) == 0);
	get_eth_mock(4);

	ASSERT(fifo_get_count(&can2udp_infifo) == 0);
	ASSERT(fifo_get_count(&can2udp_outfifo) == 0);
	ASSERT(fifo_get_count(&eth_outfifo) == 0);

	ASSERT(framestorage_item_next() == 0);


	PASS();
}


SUITE(suite) {
	RUN_TEST(fifo_case);
	RUN_TEST(framestorage_case);
	RUN_TEST(send_udp_msg_case);
	RUN_TEST(send_can2udp_case);
	RUN_TEST(recv_can2udp_case);

}

/* Add all the definitions that need to be in the test runner's main file. */
GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();      /* command-line arguments, initialization. */
    RUN_SUITE(suite);
    GREATEST_MAIN_END();        /* display results */
}
