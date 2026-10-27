#ifndef CAN_H
#define CAN_H

#include "common.h"
#include <stdint.h>

void can_transmit(struct CAN_Message *message_data);
struct CAN_Message can_wait_receive();
struct CAN_Message can_debug();
uint8_t can_init();
uint8_t can_read(uint8_t address);
void can_write(uint8_t data, uint8_t address);
void request_to_send(uint8_t buffer);
uint8_t read_status();
void bit_modify(uint8_t address, uint8_t mask, uint8_t data_byte);
void reset();
#endif // CAN_H
