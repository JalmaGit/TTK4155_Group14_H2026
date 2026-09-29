#include "spi.h"
#include "common.h"
#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>

#define MOSI_PIN PB5
#define SCK_PIN PB7

#define IO_CS = 0
#define OLED = 1


void spi_master_init() {
  /* set MOSI and SCK output, all other input
    set PB4 as output, otherwise it can be used to override SPI-master mode */
  set_bit(DDRB, PB4);
  set_bit(DDRB, MOSI_PIN);
  set_bit(DDRB, SCK_PIN);

  /* Enable SPI, Master, set clock rate fck/16 */
  set_bit(SPCR, MSTR);
  set_bit(SPCR, SPE);
  set_bit(SPCR, SPR0);

  /* Set CS pins as output */
  set
}

bool is_complete() { return test_bit(SPSR, SPIF); }

uint8_t spi_master_transceive(uint8_t data, volatile uint8_t *port,
                              uint8_t pin) {
  clear_bit(*port, pin);
  SPDR = data;
  while (!is_complete()) {
  }
  set_bit(*port, pin);
  return SPDR;
}

uint8_t spi_transcieve_bytes(uint8_t n_bytes, uint8_t data, volatile uint8_t *port, uint8_t pin)
