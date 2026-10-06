#include "adc.h"
#include "board.h"
#include "display/oled.h"
#include "spi.h"
#include "uart.h"
#include "ui/menu_items.h"
#include "user_io.h"
#include "xmem.h"
#include <util/delay.h>

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

  menu_main();

  while (1) {
    _delay_ms(100);
  }
}
