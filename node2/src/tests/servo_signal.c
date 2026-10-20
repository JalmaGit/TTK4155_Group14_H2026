#include "sam.h"
#include "time.h"
#include <stdint.h>
#include <stdio.h>
#include "board.h"
/*
Exercise 3.6.5 1) Sets the servo header signal pin to high and verify with
oscilloscope
*/

void test_servo_signal() {

  PIOB->PIO_CODR = SERVO_SIGNAL_PIN; // Set putput LOW
  PIOB->PIO_OER = SERVO_SIGNAL_PIN;  // Enable output
  PIOB->PIO_PER = SERVO_SIGNAL_PIN;  // Give PIO control of the pin

  for (int i = 0; i <= 20; i++) {
    PIOB->PIO_SODR = SERVO_SIGNAL_PIN; // Set HIGH
    printf("Servo SIG: HIGH\r\n");
    time_spinFor(msecs(1000));

    PIOB->PIO_CODR = SERVO_SIGNAL_PIN; // Set LOW
    printf("Servo SIG: LOW\r\n");
    time_spinFor(msecs(1000));
  }
}