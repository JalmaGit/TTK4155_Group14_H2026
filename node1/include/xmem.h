// This module is responsible for handling external memory for the ATmega162.

#include <stdint.h>

// Start address for the SRAM.
#define EXT_MEM_START 0x1400

void xmem_init(void);

void xmem_write(uint8_t data, uint16_t addr);

uint8_t xmem_read(uint16_t addr);
