#include "spi.h"
#include "common.h"
#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>

#define MOSI_PB5 DDB5
#define SCK_PB7 DDB7

void spi_master_init() {
  /* set MOSI and SCK output, all other input */
  // set_bit(PORTB, DDB4);
  set_bit(DDRB, DDB4);
  set_bit(DDRB, MOSI_PB5);
  set_bit(DDRB, SCK_PB7);

  /* Enable SPI, Master, set clock rate fck/16 */
  set_bit(SPCR, MSTR);
  set_bit(SPCR, SPE);
  set_bit(SPCR, SPR0);
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
