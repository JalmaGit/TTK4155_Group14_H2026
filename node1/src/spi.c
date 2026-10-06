#include "spi.h"
#include "common.h"
#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>

#define MOSI_PIN PB5
#define SCK_PIN PB7

#define DISP_CS_PIN PB0
#define IO_CS_PIN PB1
#define CAN_CS_PIN PB4

#define IO_CS 0
#define DISP_CS 1
#define CAN_CS 2

void spi_master_init() {
  /* set MOSI and SCK output, all other input
    set PB4 as output, otherwise it can be used to override SPI-master mode */
  set_bit(DDRB, PB4);
  set_bit(DDRB, MOSI_PIN);
  set_bit(DDRB, SCK_PIN);
  clear_bit(DDRB, PB6); // Set MISO pin to output

  /* First set CS pins to HIGH, then set CS pins as output, IO_CS=PB1.
   * DISP_CS=PB0 */
  set_bit(PORTB, PB1);
  set_bit(PORTB, PB0);
  set_bit(DDRB, IO_CS_PIN);
  set_bit(DDRB, DISP_CS_PIN);

  /* Enable SPI, Master, set clock rate fck/16 */
  set_bit(SPCR, MSTR);
  set_bit(SPCR, SPR0);
  set_bit(SPCR, SPE);
}

bool is_complete() { return test_bit(SPSR, SPIF); }

void select_device(uint8_t CS) {
  switch (CS) {
  case (IO_CS):
    clear_bit(PORTB, IO_CS_PIN);
    break;
  case (DISP_CS):
    clear_bit(PORTB, DISP_CS_PIN);
    break;
  case (CAN_CS):
    clear_bit(PORTB, CAN_CS_PIN);
    break;
  };
}

void deselect_device(uint8_t CS) {

  switch (CS) {
  case (IO_CS):
    set_bit(PORTB, IO_CS_PIN);
    break;
  case (DISP_CS):
    set_bit(PORTB, DISP_CS_PIN);
    break;
  case (CAN_CS):
    set_bit(PORTB, CAN_CS_PIN);
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

void spi_write(uint8_t data){
  spi_master_transceive(data);
}

uint8_t spi_read(){
  return spi_master_transceive(0);
}