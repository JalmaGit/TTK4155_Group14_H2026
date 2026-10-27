#include "can/mcp2515_high_level.h"
#include "board.h"
#include "can.h"
#include "can/config.h"
#include "can/mcp2515.h"
#include "can/mcp2515_low_level.h"
#include "common.h"
#include <spi.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>

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

struct CAN_Debug_Message can_debug() {
  struct CAN_Debug_Message message_data;
  message_data.read_status = read_status();
  message_data.can_status = can_read(MCP_CANSTAT);
  message_data.can_interrupt_flag = can_read(MCP_CANINTF);
  message_data.error_flag = can_read(MCP_EFLG);
  message_data.transmitter_error_flag = can_read(MCP_TEC);
  message_data.receiver_error_flag = can_read(MCP_REC);

  return message_data;
}

struct CAN_Message can_wait_receive() {
  uint8_t INTERRUP_PIN_VALUE = test_bit(CAN_INT_PIN_PORT, CAN_INT_PIN);
  uint8_t buffers_status = 0x00;
  struct CAN_Message message_data;
  message_data.ID = 0xFFFF; // This is an invalid ID
  message_data.data_length = 0;
  message_data.data[0] = 0;

  if (!INTERRUP_PIN_VALUE) {
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

void enable_loop_back_mode() { mode_configure(CAN_LOOP_BACK_MODE); }
void disable_loop_back_mode() { mode_configure(CAN_NORMAL_MODE); }

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
  interrupt_configure();
  mode_configure(CAN_NORMAL_MODE);

  return 0;
}
