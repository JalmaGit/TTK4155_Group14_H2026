#include "lights.h"
#include "uart.h"
#include "xmem.h"
#include <avr/io.h>
#include <stdio.h>
#include <tests/sram_test.h>
#include <util/delay.h>

// System Oscillator clock frequency
#define FOSC F_CPU
// bits per second
#define BAUD 9600
// Table 70 in ATmega162 datasheet
#define MY_UBRR FOSC / 16 / BAUD - 1

int main() {
  // Pin A7 for Status LED

  // Set outputs
  DDRA = 0b10000000;

  xmem_init();
  uart_init(MY_UBRR);

  status_light(1);

  SRAM_test();

  printf("\nHello world!\n");
  while (1) {
    uart_transmit(uart_receive());
    status_light(1);

    uart_transmit(uart_receive());
    status_light(0);
  }
}
