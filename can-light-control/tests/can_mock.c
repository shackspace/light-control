#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "greatest.h"

#include "../can.h"
#include "../fifo.h"
#include "../shackbus.h"



fifo_t mock_can_outfifo;
uint8_t outbuf[100];
fifo_t mock_can_infifo;
uint8_t inbuf[100];

bool can_mock_send_message_enable = true;

void can_mock_init(void);

void can_mock_init(void)
{
	fifo_init (&mock_can_outfifo,   outbuf, 100);
	fifo_init (&mock_can_infifo,   inbuf, 100);
	can_mock_send_message_enable = true;
	return;
}

bool can_check_message(void)
{
	if ( fifo_get_count(&mock_can_infifo) )
		return true;
	return false;
};

extern uint8_t mcp2515_read_status(uint8_t type);
extern uint8_t mcp2515_read_status(uint8_t type)
{
	return 2;
}

uint8_t can_get_message(can_t *msg)
{
	fifo_get_var(&mock_can_infifo, (uint8_t *)&msg->id, 4);
	(*msg).flags.rtr = fifo_get(&mock_can_infifo);
	(*msg).flags.extended = fifo_get(&mock_can_infifo);
	msg->length = fifo_get(&mock_can_infifo);
	fifo_get_var(&mock_can_infifo, msg->data, 8);
};




uint8_t can_input_message(const can_t *msg);

uint8_t can_input_message(const can_t *msg)
{
	fifo_put_var(&mock_can_infifo, (uint8_t *)&msg->id, 4);
	fifo_put(&mock_can_infifo, (*msg).flags.rtr);
	fifo_put(&mock_can_infifo, (*msg).flags.extended);
	fifo_put_var(&mock_can_infifo, &msg->length, 1);
	fifo_put_var(&mock_can_infifo, &msg->data, 8);
};



uint8_t can_send_message(const can_t *msg)
{
//	fifo_put_var(&mock_can_outfifo, (uint8_t *)&msg, sizeof(can_t));
	fifo_put_var(&mock_can_outfifo, (uint8_t *)&msg->id, 4);
	fifo_put(&mock_can_outfifo, (*msg).flags.rtr);
	fifo_put(&mock_can_outfifo, (*msg).flags.extended);
	fifo_put_var(&mock_can_outfifo, &msg->length, 1);
	fifo_put_var(&mock_can_outfifo, &msg->data, 8);

	return can_mock_send_message_enable;

};

bool can_compare_sended(const can_t msg);
bool can_compare_sended(const can_t msg)
{
	can_t tmp_msg;
	fifo_get_var(&mock_can_outfifo, (uint8_t *)&tmp_msg.id, 4);
	tmp_msg.flags.rtr = fifo_get(&mock_can_outfifo);
	tmp_msg.flags.extended = fifo_get(&mock_can_outfifo);
	tmp_msg.length = fifo_get(&mock_can_outfifo);
	fifo_get_var(&mock_can_outfifo, tmp_msg.data, 8);


	if ( (tmp_msg.id & 0x7FFFFFFF) != (msg.id & 0x7FFFFFFF) ) return false;
	else if (tmp_msg.flags.rtr != msg.flags.rtr) return false;
	else if (tmp_msg.flags.extended != msg.flags.extended) return false;
	else if (tmp_msg.length != msg.length) return false;
	else if (tmp_msg.flags.rtr != msg.flags.rtr) return false;
	else if (memcmp(tmp_msg.data, msg.data, msg.length) != 0 ) return false;

	return true;
};



can_error_register_t can_read_error_register(void){};
bool can_check_free_buffer(void){};
bool can_init(can_bitrate_t bitrate){return;};
void can_static_filter(const uint8_t *filter_array){};

