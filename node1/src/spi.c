#include "spi.h"
#include "board.h"
#include "common.h"
#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>

void spi_master_init() {
  /* set MOSI and SCK output, all other input
    set SPI SS (PB4) as output, otherwise it can be used to override SPI-master
    mode */
  set_bit(SPI_DDR, SPI_SS_BIT);
  set_bit(SPI_DDR, SPI_MOSI_BIT);
  set_bit(SPI_DDR, SPI_SCK_BIT);
  clear_bit(SPI_DDR, SPI_MISO_BIT); // Set MISO pin to input

  /* First set CS pins to HIGH, then set CS pins as output, IO_CS=PB1.
   * OLED_CS=PB0 */
  set_bit(SPI_PORT, CS_IO_BIT);
  set_bit(SPI_PORT, CS_DISP_BIT);
  set_bit(SPI_PORT, CS_CAN_BIT);
  set_bit(SPI_DDR, CS_IO_BIT);
  set_bit(SPI_DDR, CS_DISP_BIT);
  set_bit(SPI_DDR, CS_CAN_BIT);

  /* Enable SPI, Master, set clock rate fck/16 */
  set_bit(SPCR, MSTR);
  set_bit(SPCR, SPR0);
  set_bit(SPCR, SPE);
}

bool is_complete() { return test_bit(SPSR, SPIF); }

void select_device(uint8_t CS) {
  switch (CS) {
  case (CS_ID_IO):
    clear_bit(CS_PORT, CS_IO_BIT);
    break;
  case (CS_ID_DISP):
    clear_bit(CS_PORT, CS_DISP_BIT);
    break;
  case (CS_ID_CAN):
    clear_bit(PORTB, CS_CAN_BIT);
    break;
  };
}

void deselect_device(uint8_t CS) {

  switch (CS) {
  case (CS_ID_IO):
    set_bit(CS_PORT, CS_IO_BIT);
    break;
  case (CS_ID_DISP):
    set_bit(CS_PORT, CS_DISP_BIT);
    break;
  case (CS_ID_CAN):
    set_bit(PORTB, CS_CAN_BIT);
    break;
  };
}

uint8_t spi_master_transceive(uint8_t data) {
  SPDR = data;
  while (!is_complete()) {
  }

  return SPDR;
}

uint8_t spi_transceive_byte(uint8_t data, uint8_t CS) {
  select_device(CS);
  uint8_t received_data = spi_master_transceive(data);
  deselect_device(CS);
  return received_data;
}

void spi_transceive_bytes(uint8_t n_bytes, uint8_t *send_data,
                          uint8_t *received_data, uint8_t CS) {
  select_device(CS);
  for (uint8_t byte = 0; byte < n_bytes; byte++) {
    received_data[byte] = spi_master_transceive(send_data[byte]);
  }
  deselect_device(CS);
}
