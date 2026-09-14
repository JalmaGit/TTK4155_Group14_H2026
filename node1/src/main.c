#include "adc.h"
#include "lights.h"
#include "uart.h"
#include "xmem.h"
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

  _delay_ms(1000);

  SRAM_test();

  while (1) {
    _delay_ms(500);
  }
}
