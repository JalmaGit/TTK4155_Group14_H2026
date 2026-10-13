#ifndef SPI_H
#define SPI_H

#include <stdint.h>

void spi_master_init();
uint8_t spi_transceive_byte(uint8_t data, uint8_t CS);
void spi_transceive_bytes(uint8_t n_bytes, uint8_t *send_data,
                          uint8_t *received_data, uint8_t CS);

// Advanced Use
void select_device(uint8_t CS);
void deselect_device(uint8_t CS);
uint8_t spi_master_transceive(uint8_t data);
uint8_t spi_read();
void spi_write(uint8_t data);

#endif // SPI_H
