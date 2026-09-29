#ifndef SPI_H
#define SPI_H

#include <stdint.h>

void spi_master_init();
uint8_t spi_transceive_byte(uint8_t data, uint8_t CS);
void spi_transceive_bytes(uint8_t n_bytes, uint8_t *send_data, uint8_t *received_data, uint8_t CS);

#endif // SPI_H
