#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <avr/io.h>

#include "greatest.h"

#include "../config.h"
#include "../stack.h"
#include "../fifo.h"

fifo_t eth_outfifo;
static uint8_t outbuf[100];


void kill_udp_app(unsigned int port){};
void add_udp_app(unsigned int port, void(*fp1)(unsigned char)){};
unsigned char eth_buffer[MTU_SIZE+1];
void create_new_udp_packet( unsigned int  data_length, unsigned int  src_port, unsigned int  dest_port, unsigned long dest_ip){
	fifo_put_var (&eth_outfifo, &eth_buffer[UDP_DATA_START+0], 4);
};
char arp_request (unsigned long var){};
char arp_entry_search (unsigned long var){};


unsigned char myip[4];
unsigned char netmask[4];
unsigned char router_ip[4];
unsigned char broadcast_ip[4];

void clean_eth_buffer(void);
void clean_eth_buffer(void)
{
	memset(eth_buffer,0,MTU_SIZE+1);
}
void init_eth_mock(void);
void init_eth_mock(void)
{
	fifo_init (&eth_outfifo,   outbuf, 100);
}


