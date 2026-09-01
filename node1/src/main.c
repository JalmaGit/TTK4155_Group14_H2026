#include <avr/io.h>
#include <util/delay.h>

#include "lights.h"

int main() {
  // Pin A7 for Status LED

  // Set outputs
  DDRA = 0b10000000;

  status_light(1);

  while (1) {
    status_light(1);
    _delay_ms(500);
    status_light(0);
    _delay_ms(500);
  }
}
