#include "mcp2515_can.h"
#include "board.h"
#include "common.h"
#include "mcp2515.h"
#include <spi.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>

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

void can_interupt_configure() {
  clear_bit(CAN_INT_PORT, CAN_INT_PIN);

  uint8_t CANINTE_MASK = 0b00011111;
  uint8_t CANINTE_BYTE = 0b00011111;

  bit_modify(MCP_CANINTE, CANINTE_MASK, CANINTE_BYTE);
}

void low_priority_message_configure() {
  low_priority_buffer_configure();
  low_priority_mask_configure();
  low_priority_filter_configure();
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

void can_read_high_priority_message(struct CAN_Message *message_data) {
  uint8_t ID_H = can_read(MCP_RXB0SIDH);
  uint8_t ID_L = can_read(MCP_RXB0SIDL);
  message_data->ID = (((uint16_t)ID_H << 8) | ID_L) >> 5;

  uint8_t receive_buffer_length_code = can_read(MCP_RXB0DLC);
  message_data->data_length = receive_buffer_length_code & 0x0F;

  uint8_t RXB0DM_ADRESS = MCP_RXB0DM_START_ADDRESS;
  for (uint8_t i = 0; i < message_data->data_length; i++) {
    message_data->data[i] = can_read(RXB0DM_ADRESS);
    RXB0DM_ADRESS++;
  }

  uint8_t RX0IF_RESET_BYTE = 0x00;
  bit_modify(MCP_CANINTF, MCP_RX0IF, RX0IF_RESET_BYTE);
}

void can_read_low_priority_message(struct CAN_Message *message_data) {
  uint8_t ID_H = can_read(MCP_RXB1SIDH);
  uint8_t ID_L = can_read(MCP_RXB1SIDL);

  message_data->ID = (((uint16_t)ID_H << 8) | ID_L) >> 5;

  uint8_t receive_buffer_length_code = can_read(MCP_RXB1DLC);
  message_data->data_length = receive_buffer_length_code & 0x0F;

  uint8_t RXB1DM_ADRESS = MCP_RXB1DM_START_ADDRESS;
  for (uint8_t i = 0; i < message_data->data_length; i++) {
    message_data->data[i] = can_read(RXB1DM_ADRESS);
    RXB1DM_ADRESS++;
  }

  uint8_t RX0IF_RESET_BYTE = 0x00;
  bit_modify(MCP_CANINTF, MCP_RX1IF, RX0IF_RESET_BYTE);
}

struct CAN_Message can_debug() {
  struct CAN_Message message_data;
  message_data.ID = (uint16_t)read_status();
  message_data.data_length = can_read(MCP_CANSTAT);
  message_data.data[0] = can_read(MCP_CANINTF);

  return message_data;
}

struct CAN_Message can_wait_receive() {
  uint8_t PIN_VALUE = PIND;
  uint8_t buffers_status = 0x00;
  struct CAN_Message message_data;
  message_data.ID = 0xFFFF;
  message_data.data_length = 0;
  message_data.data[0] = 0;

  if (!test_bit(PIN_VALUE, PIND2)) {
    buffers_status = read_status();
  }

  if (test_bit(buffers_status, 0)) {
    can_read_high_priority_message(&message_data);
  } else if (test_bit(buffers_status, 1)) {
    can_read_low_priority_message(&message_data);
  }

  return message_data;
}

void can_transmit_buffer0_write(struct CAN_Message *message_data) {
  uint8_t ID_H = (uint8_t)(message_data->ID >> 3);
  uint8_t ID_L = (uint8_t)(message_data->ID << 5);

  can_write(ID_H, MCP_TXB0SIDH);
  can_write(ID_L, MCP_TXB0SIDL);
  can_write(message_data->data_length, MCP_TXB0DLC);

  uint8_t TXB0DM_ADRESS = MCP_TXB0DM_START_ADDRESS;
  for (uint8_t i = 0; i < message_data->data_length; i++) {
    can_write(message_data->data[i], TXB0DM_ADRESS);
    TXB0DM_ADRESS++;
  }

  request_to_send(MCP_RTS_TX0);
}

void can_transmit_buffer1_write(struct CAN_Message *message_data) {
  uint8_t ID_H = (uint8_t)(message_data->ID >> 3);
  uint8_t ID_L = (uint8_t)(message_data->ID << 5);

  can_write(ID_H, MCP_TXB1SIDH);
  can_write(ID_L, MCP_TXB1SIDL);
  can_write(message_data->data_length, MCP_TXB1DLC);

  uint8_t TXB1DM_ADRESS = MCP_TXB1DM_START_ADDRESS;
  for (uint8_t i = 0; i < message_data->data_length; i++) {
    can_write(message_data->data[i], TXB1DM_ADRESS);
    TXB1DM_ADRESS++;
  }

  request_to_send(MCP_RTS_TX1);
}

void can_transmit_buffer2_write(struct CAN_Message *message_data) {
  uint8_t ID_H = (uint8_t)(message_data->ID >> 3);
  uint8_t ID_L = (uint8_t)(message_data->ID << 5);

  can_write(ID_H, MCP_TXB2SIDH);
  can_write(ID_L, MCP_TXB2SIDL);
  can_write(message_data->data_length, MCP_TXB2DLC);

  uint8_t TXB2DM_ADRESS = MCP_TXB2DM_START_ADDRESS;
  for (uint8_t i = 0; i < message_data->data_length; i++) {
    can_write(message_data->data[i], TXB2DM_ADRESS);
    TXB2DM_ADRESS++;
  }

  request_to_send(MCP_RTS_TX2);
}

void can_transmit(struct CAN_Message *message_data) {
  uint8_t buffers_status = read_status();

  if (!test_bit(buffers_status, 2)) {
    bit_modify(MCP_CANINTF, MCP_TX0IF, 0x00);
    can_transmit_buffer0_write(message_data);
  } else if (!test_bit(buffers_status, 4)) {
    bit_modify(MCP_CANINTF, MCP_TX1IF, 0x00);
    can_transmit_buffer1_write(message_data);
  } else if (!test_bit(buffers_status, 6)) {
    bit_modify(MCP_CANINTF, MCP_TX2IF, 0x00);
    can_transmit_buffer2_write(message_data);
  }
}

uint8_t can_init() {
  uint8_t value;
  spi_master_init();
  reset();
  _delay_ms(100);
  value = can_read(MCP_CANSTAT);
  if ((value & MODE_MASK) != MODE_CONFIG) {
    printf(" MCP2515 is not in confiugration mode after reset \r\n");
    return 1;
  }
  bit_timings_configure();
  receive_configure();
  can_interupt_configure();
  mode_configure();

  return 0;
}

uint8_t can_read(uint8_t address) {
  uint8_t result;
  select_device(CS_ID_CAN);
  spi_master_transceive(MCP_READ);
  spi_master_transceive(address);
  result = spi_master_transceive(0xFF);
  deselect_device(CS_ID_CAN);
  return result;
}

void can_write(uint8_t data, uint8_t address) {
  select_device(CS_ID_CAN);
  spi_master_transceive(MCP_WRITE);
  spi_master_transceive(address);
  spi_master_transceive(data);
  deselect_device(CS_ID_CAN);
}

void reset() {
  select_device(CS_ID_CAN);
  spi_master_transceive(MCP_RESET);
  deselect_device(CS_ID_CAN);
}

void request_to_send(uint8_t buffer) {
  select_device(CS_ID_CAN);
  spi_master_transceive(buffer);
  deselect_device(CS_ID_CAN);
}

void bit_modify(uint8_t address, uint8_t mask, uint8_t data_byte) {
  select_device(CS_ID_CAN);
  spi_master_transceive(MCP_BITMOD);
  spi_master_transceive(address);
  spi_master_transceive(mask);
  spi_master_transceive(data_byte);
  deselect_device(CS_ID_CAN);
}

uint8_t read_status() {
  uint8_t status;
  select_device(CS_ID_CAN);
  spi_master_transceive(MCP_READ_STATUS);
  status = spi_master_transceive(0xFF);
  deselect_device(CS_ID_CAN);
  return status;
}
