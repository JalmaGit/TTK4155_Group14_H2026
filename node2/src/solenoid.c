#include "solenoid.h"
#include "board.h"
#include "sam.h"
#include <stdbool.h>

void solenoid_pin_low(void) {
  SOLENOID_SIGNAL_BLOCK->PIO_CODR = SOLENOID_SIGNAL_PIN;
}

void solenoid_pin_high(void) {
  SOLENOID_SIGNAL_BLOCK->PIO_SODR = SOLENOID_SIGNAL_PIN;
}

void solenoid_retract(void) { solenoid_pin_high(); }

void solenoid_extend(void) { solenoid_pin_low(); }

void solenoid_init(void) {
  PMC->PMC_PCER0 = (1u << ID_PIOC); // Enable perhiperal clock
  solenoid_retract();
  // Enable output
  SOLENOID_SIGNAL_BLOCK->PIO_OER = SOLENOID_SIGNAL_PIN;
  // Give PIO control of the pin
  SOLENOID_SIGNAL_BLOCK->PIO_PER = SOLENOID_SIGNAL_PIN;
}

void solenoid_set(bool extended) {
  if (extended) {
    solenoid_extend();
  } else {
    solenoid_retract();
  }
}
