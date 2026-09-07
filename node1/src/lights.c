#include "lights.h"

#include "common.h"

void status_light(uint8_t state) {
  if (state == 1) {
    set_bit(PORTA, PA7);
  } else {
    clear_bit(PORTA, PA7);
  }
}

void status_light_init(void) {
  set_bit(DDRA, PA7);
  clear_bit(PORTA, PA7);
}
