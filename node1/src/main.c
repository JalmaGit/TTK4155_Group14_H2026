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
  joy_calibrate(); // Joystick needs to be in neutral position

  _delay_ms(1000);
  
  while (1) {

    switch (joy_read_dir()) {
      case JOY_LEFT:
        printf("LEFT\r\n");
        break;
      case JOY_RIGHT:
        printf("RIGHT\r\n");
        break;
      case JOY_UP:
        printf("UP\r\n");
        break;
      case JOY_DOWN:
        printf("DOWN\n");
        break;
      case JOY_NEUTRAL:
        printf("NEUTRAL\n");
        break;
    }
    _delay_ms(1000);

  }
  */
}
