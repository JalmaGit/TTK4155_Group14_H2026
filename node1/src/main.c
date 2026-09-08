#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#include "lights.h"
#include "uart.h"

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

  uart_init(MY_UBRR);

  status_light(1);

  printf("\nHello world!\n");
  while (1) {
    uart_transmit(uart_receive());
    status_light(1);

    uart_transmit(uart_receive());
    status_light(0);
  }
}
