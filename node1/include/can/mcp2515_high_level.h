#ifndef CAN_HIGH_LEVEL_H
#define CAN_HIGH_LEVEL_H

#include "can.h"
#include <stdint.h>

void can_transmit(struct CAN_Message *message_data);
struct CAN_Message can_wait_receive();
struct CAN_Debug_Message can_debug();
void enable_loop_back_mode();
uint8_t can_init();

#endif // CAN_HIGH_LEVEL_H
