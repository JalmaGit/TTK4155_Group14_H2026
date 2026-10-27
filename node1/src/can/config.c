#include "can/config.h"
#include "board.h"
#include "can/config.h"
#include "can/mcp2515.h"
#include "can/mcp2515_low_level.h"
#include "common.h"
#include "stdint.h"

void bit_timings_configure() {
  // This config assumes a CAN baudrate of 500kpbs, with 16 Tq available
  // PS1 = 6 Tq
  // PS2 = 7 Tq
  // PropSeg = 2 Tq
  // SyncSeg = 1 Tq
  // Ntq = PS1 + PS2 + PropSeg + SyncSeg = 16 Tq

  // bit mask to only expose specific bytes for modification for each
  // timing register
  uint8_t CNF1_MASK = 0xFF;
  uint8_t CNF2_MASK = 0xFF;
  uint8_t CNF3_MASK = 0b11000111;

  // bit values
  uint8_t CNF1_BYTE = 0x00;
  uint8_t CNF2_BYTE = 0b10101001;
  uint8_t CNF3_BYTE = 0b00000110;

  bit_modify(MCP_CNF1, CNF1_MASK, CNF1_BYTE);
  bit_modify(MCP_CNF2, CNF2_MASK, CNF2_BYTE);
  bit_modify(MCP_CNF3, CNF3_MASK, CNF3_BYTE);
}

void high_priority_buffer_configure() {
  uint8_t RXB0CTRL_MASK = 0b01100100;
  uint8_t RXB0CTRL_BYTE = 0b00100000;

  bit_modify(MCP_RXB0CTRL, RXB0CTRL_MASK, RXB0CTRL_BYTE);
}

void high_priority_mask_configure() {
  uint8_t RXM0SIDH_BYTE = 0xFF;
  uint8_t RXM0SIDL_BYTE = 0b11100000;

  can_write(RXM0SIDH_BYTE, MCP_RXM0SIDH);
  can_write(RXM0SIDL_BYTE, MCP_RXM0SIDL);
}

void high_priority_filter_configure() {
  // Filter 0
  uint8_t RXF0SIDH_BYTE = 0x00;
  uint8_t RXF0SIDL_BYTE = 0b00100000;

  can_write(RXF0SIDH_BYTE, MCP_RXF0SIDH);
  can_write(RXF0SIDL_BYTE, MCP_RXF0SIDL);

  // Filter 1
  uint8_t RXF1SIDH_BYTE = 0x00;
  uint8_t RXF1SIDL_BYTE = 0b00000000;

  can_write(RXF1SIDH_BYTE, MCP_RXF1SIDH);
  can_write(RXF1SIDL_BYTE, MCP_RXF1SIDL);
}

void high_priority_message_configure() {
  high_priority_buffer_configure();
  high_priority_mask_configure();
  high_priority_filter_configure();
}

void low_priority_buffer_configure() {
  uint8_t RXB1CTRL_MASK = 0b01100000;
  uint8_t RXB1CTRL_BYTE = 0b00100000;

  bit_modify(MCP_RXB1CTRL, RXB1CTRL_MASK, RXB1CTRL_BYTE);
}

void low_priority_mask_configure() {
  uint8_t RXM1SIDH_BYTE = 0xFF;
  uint8_t RXM1SIDL_BYTE = 0b11100000;

  can_write(RXM1SIDH_BYTE, MCP_RXM1SIDH);
  can_write(RXM1SIDL_BYTE, MCP_RXM1SIDL);
}

void low_priority_filter_configure() {
  // Filter 2
  uint8_t RXF2SIDH_BYTE = 0x00;
  uint8_t RXF2SIDL_BYTE = 0b10000000;

  can_write(RXF2SIDH_BYTE, MCP_RXF2SIDH);
  can_write(RXF2SIDL_BYTE, MCP_RXF2SIDL);

  // Filter 3
  uint8_t RXF3SIDH_BYTE = 0x00;
  uint8_t RXF3SIDL_BYTE = 0b10100000;

  can_write(RXF3SIDH_BYTE, MCP_RXF3SIDH);
  can_write(RXF3SIDL_BYTE, MCP_RXF3SIDL);

  // Filter 4
  uint8_t RXF4SIDH_BYTE = 0x00;
  uint8_t RXF4SIDL_BYTE = 0b11000000;

  can_write(RXF4SIDH_BYTE, MCP_RXF4SIDH);
  can_write(RXF4SIDL_BYTE, MCP_RXF4SIDL);

  // Filter 5
  uint8_t RXF5SIDH_BYTE = 0x00;
  uint8_t RXF5SIDL_BYTE = 0b11100000;

  can_write(RXF5SIDH_BYTE, MCP_RXF5SIDH);
  can_write(RXF5SIDL_BYTE, MCP_RXF5SIDL);
}

void low_priority_message_configure() {
  low_priority_buffer_configure();
  low_priority_mask_configure();
  low_priority_filter_configure();
}

void interrupt_configure() {
  clear_bit(CAN_INT_PORT, CAN_INT_PIN);

  uint8_t CANINTE_MASK = 0b00011111;
  uint8_t CANINTE_BYTE = 0b00011111;

  bit_modify(MCP_CANINTE, CANINTE_MASK, CANINTE_BYTE);
}

void receive_configure() {
  high_priority_message_configure();
  low_priority_message_configure();
}

void mode_configure() {
  uint8_t CANCTRL_MASK = 0b11101000;
  uint8_t CANCTRL_BYTE = 0b01000000;

  bit_modify(MCP_CANCTRL, CANCTRL_MASK, CANCTRL_BYTE);
}
