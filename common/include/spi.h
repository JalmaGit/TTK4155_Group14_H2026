#ifndef SPI_H
#define SPI_H

#include <stdint.h>

void spi_master_init(void);
uint8_t spi_master_transceive(uint8_t data, volatile uint8_t *port,
                              uint8_t pin);

#endif // SPI_H
