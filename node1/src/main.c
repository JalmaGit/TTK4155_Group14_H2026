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

  uint8_t data = 16;
  uint8_t return_data;

  _delay_ms(500);

  printf("Starting Loop \n\r");

  printf("SPCR register : %d", SPCR);
  printf("DDRB register : %d", DDRB);

  printf("SPCR=%d DDRB=0x%02X PINB=0x%02X SPSR=%d\n\r", SPCR, DDRB, PINB, SPSR);

  while (1) {
    return_data = spi_master_transceive(data, &PORT_PB, IO_CS);
    _delay_ms(500);
    printf("SPI return data: %d\n\r", return_data);
  }

  /*
  xmem_initem_init();
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
    for (int ch = 0; ch <= 4; ch++) {
      uint8_t value = adc_read(0);
      printf("adc (%i): %d\n", ch, value);
    }
    _delay_ms(2000);

    // Read from memory again
    uint8_t value = xmem_read(0x0);
    printf("mem: %d\n", value);
    _delay_ms(2000);
  }

  while (1) {
    _delay_ms(500);
  }
  */
}
