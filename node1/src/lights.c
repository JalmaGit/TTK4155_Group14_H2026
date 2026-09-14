#include "lights.h"

#include "common.h"
#include <avr/io.h>

// TODO: Confirm that this pin does not conflict with other devices.
void status_light_init(void) {
  // Mark pin as "output"
  set_bit(DDRA, PA7);

  // Default Off
  status_light(0);
}

void status_light(uint8_t state) {
  if (state == 1) {
    set_bit(PORTA, PA7);
  } else {
    clear_bit(PORTA, PA7);
  }
}
