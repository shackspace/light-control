#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "greatest.h"

#include "../can.h"

bool can_check_message(void){};
uint8_t can_get_message(can_t *msg){};
uint8_t can_send_message(const can_t *msg){};
can_error_register_t can_read_error_register(void){};
bool can_check_free_buffer(void){};


