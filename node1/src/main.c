#include "adc.h"
#include "lights.h"
#include "uart.h"
#include "user_io.h"
#include "xmem.h"
#include <stdint.h>
#include <tests/sram_test.h>
#include <util/delay.h>

// System Oscillator clock frequency
#define FOSC F_CPU
// bits per second
#define BAUD 9600
// Table 70 in ATmega162 datasheet
#define MY_UBRR FOSC / 16 / BAUD - 1

int main() {
  xmem_init();
  adc_init();
  uart_init(MY_UBRR);

  joy_calibrate();

  _delay_ms(1000);

  // SRAM_test();

  // printf("SRAM TEST DONE\n");

  _delay_ms(1000);

  // Try to get adc data
  // uint8_t value = adc_read(1);
  // printf("adc: %d", value);

  while (1) {
    uint8_t value = adc_read(0);
    printf("adc: %d\n", value);
    _delay_ms(2000);

    // Read from memory again
    value = xmem_read(0x0);
    printf("mem: %d\n", value);
    _delay_ms(2000);
  }

  while (1) {
    _delay_ms(500);
  }
}
