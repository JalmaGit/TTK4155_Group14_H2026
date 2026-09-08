#include "lights.h"

#include "common.h"

void status_light(uint8_t state) {
  if (state == 1) {
    set_bit(PORTA, PA7);
  } else {
    clear_bit(PORTA, PA7);
  }
}
