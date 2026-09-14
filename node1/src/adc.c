#include "adc.h"
#include "xmem.h"

void adc_init(void) {
  // TODO: Figure out how data is to be read + timing details.
  // TODO: Configure ADC Clock
  // TODO: Connect Clock to ADC
}

// TODO: Verify this is 8 bits.
uint8_t adc_read(void) {
  // TODO: Verify that ADC data can be read from arbitrary address between
  // 0x1000 and 0x13FF
  return xmem_read(0x1000);
}
