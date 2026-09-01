#include <avr/io.h>

#define F_CPU 4915200
#include <util/delay.h>

#define set_bit(reg, bit) (reg |= (1 << bit))
#define clear_bit(reg, bit) (reg &= ~(1 << bit))
#define test_bit(reg, bit) (reg & (1 << bit))

void status_light(uint8_t state) {
  if (state == 1) {
    set_bit(PORTA, PA7);
  } else {
    clear_bit(PORTA, PA7);
  }
}

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
