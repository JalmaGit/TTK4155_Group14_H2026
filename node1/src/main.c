#include "adc.h"
#include "display/oled.h"
#include "lights.h"
#include "spi.h"
#include "uart.h"
#include "user_io.h"
#include "xmem.h"
#include <avr/io.h>
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

  _delay_ms(500);

  xmem_init();
  adc_init();
  joy_calibrate(); // Joystick needs to be in neutral position

  _delay_ms(1000);

  oled_init();

  _delay_ms(100);

  oled_test();
}
