#include "tests/led_test.h"
#include "user_io.h"

#include <stdbool.h>
#include <util/delay.h>

void led_test(void) {
  led_init();

  _delay_ms(200);

  for (int i = 0; i < 6; i++) {
    set_led(i, 1);
    _delay_ms(200);
  }

  for (int i = 0; i < 6; i++) {
    set_led(i, 0);
    _delay_ms(200);
  }

  for (int v = 0; v < 256; v++) {
    for (int i = 0; i < 6; i++) {
      set_led_pwm(i, v);
    }
    _delay_ms(5);
  }

  for (int v = 255; v >= 0; v--) {
    for (int i = 0; i < 6; i++) {
      set_led_pwm(i, v);
    }
    _delay_ms(5);
  }

  for (int v = 0; v < 64; v++) {
    bool bit0 = (v & 0b100000);
    bool bit1 = (v & 0b10000);
    bool bit2 = (v & 0b1000);
    bool bit3 = (v & 0b100);
    bool bit4 = (v & 0b10);
    bool bit5 = (v & 0b1);

    set_led(0, bit0);
    set_led(1, bit1);
    set_led(2, bit2);
    set_led(3, bit3);
    set_led(4, bit4);
    set_led(5, bit5);
    _delay_ms(100);
  }

  _delay_ms(1000);

  led_init();
}
