// This driver is responsible for accessing the ADC which is done by reading
// from any address between 0x1000 and 0x13FF
#include <stdint.h>

void adc_init(void);

// TODO: Verify this is 8 bits.
uint8_t adc_read(void);
