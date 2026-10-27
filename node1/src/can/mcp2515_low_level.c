#include "can/mcp2515_low_level.h"
#include "board.h"
#include "can/mcp2515.h"
#include "spi.h"

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
