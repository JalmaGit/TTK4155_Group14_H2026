/// This module is responsible for reading data from the ADC.

#ifndef ADC_H
#define ADC_H

// This driver is responsible for accessing the ADC which is done by reading
// from any address between 0x1000 and 0x13FF
#include <stdint.h>

// Requires xmem to also have been initialized (to set SRE).
void adc_init(void);

uint8_t adc_read(uint8_t channel);

#endif // ADC_H
