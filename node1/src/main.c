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
}
