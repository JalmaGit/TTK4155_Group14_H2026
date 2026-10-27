#include "adc.h"
#include "board.h"
#include "display/oled.h"
#include "spi.h"
#include "uart.h"
#include "ui/menu_items.h"
#include "user_io.h"
#include "xmem.h"
#include <stdio.h>
#include <util/delay.h>

int main() {
  uart_init(MY_UBRR);
  printf("UART Initialized...\r\n");
  _delay_ms(500);
  printf("Waited 500ms... \r\n");

  init_can_bus_test();

  spi_master_init();

  _delay_ms(500);

  xmem_init();
  adc_init();
  led_init();
  joy_calibrate(); // Joystick needs to be in neutral position

  _delay_ms(1000);

  oled_init();
  printf("OLED Initialized... \r\n");

  _delay_ms(100);

  menu_main();

  while (1) {
    can_bus_test();
  }
}
