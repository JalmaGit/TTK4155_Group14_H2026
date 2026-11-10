#include "game/tick.h"
#include "common.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>

static volatile uint8_t pending_ticks;

ISR(TIMER0_COMP_vect) { pending_ticks++; }

_Static_assert(T0_OCR <= 255, "Tick too slow for 8-bit Timer0, use Timer1");

void tick_init(void) {
  // Clock source (internal) seleted in Timer/Counter Control Register (TCCR0)
  // Set internal clock source, and CTC operation mode
  TCCR0 = (1 << WGM01) | (1 << CS02) | (1 << CS00);

  // Count up to:
  OCR0 = T0_OCR;

  // Reset counter.
  TCNT0 = 0;
  // Clear stale compare flags.
  TIFR |= 1u << OCF0;
  // Drop stale ticks.
  pending_ticks = 0;

  // Enable interrupt on Compare Match
  TIMSK |= 1u << OCIE0;

  // Set global interrupt flag (allow interrupts)
  sei();
}

// Returns missed ticks (e.g. "dropped frames")
uint8_t tick_wait(void) {
  while (!pending_ticks) {
  }

  // Clear global interrupt flag (dissallow interrupts)
  cli();

  uint8_t missed = pending_ticks - 1;
  pending_ticks = 0;

  // Set global interrupt flag (allow interrupts)
  sei();

  return missed;
}

void tick_stop(void) {
  clear_bit(TIMSK, OCIE0);
  // Stop timer.
  TCCR0 = 0;
}
