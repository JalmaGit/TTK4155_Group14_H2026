#ifndef MCP2515_LOW_LEVEL_H
#define MCP2515_LOW_LEVEL_H

#include <stdint.h>

uint8_t can_read(uint8_t address);
void can_write(uint8_t data, uint8_t address);
void reset();
void request_to_send(uint8_t buffer);
void bit_modify(uint8_t address, uint8_t mask, uint8_t data_byte);
uint8_t read_status();

#endif // MCP2515_LOW_LEVEL_H
