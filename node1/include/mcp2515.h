#ifndef MCP2515_H
#define MCP2515_H

#include <stdint.h>

uint8_t mcp2515_read();
void mcp2515_write();
uint8_t mcp2515_request_to_send();
void mcp2515_bit_modify();
void mcp2515_reset();
uint8_t mcp2515_read_status();

#endif // MCP2515_H
