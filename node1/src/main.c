#include "adc.h"
#include "common.h"
#include "lights.h"
#include "spi.h"
#include "uart.h"
#include "user_io.h"
#include "xmem.h"
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <tests/sram_test.h>
#include <util/delay.h>

// System Oscillator clock frequency
#define FOSC F_CPU
// bits per second
#define BAUD 9600
// Table 70 in ATmega162 datasheet
#define MY_UBRR FOSC / 16 / BAUD - 1

#define IO_CS PB0
// #define DISP_CS PB1
#define PORT_PB PORTB

int main() {
  uart_init(MY_UBRR);
  spi_master_init();

  while (1) {

    uint8_t output_data[] = {1, 32, 45};
    uint8_t input_data[3];

    spi_transceive_bytes(3, output_data, input_data, IO_CS);

    printf("%u, %u, %u\r\n", input_data[0], input_data[1], input_data[2]);
    _delay_ms(1000);
  }
  // printf("Test 1 byte: 53");
}
