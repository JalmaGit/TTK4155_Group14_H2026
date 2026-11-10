#include "sam.h"
#include "time.h"
#include <stdint.h>
#include <stdio.h>
#include "board.h"
#include "servo.h"
/*
Exercise 3.6.5 1) Sets the servo header signal pin to high and verify with
oscilloscope
*/

void test_servo_signal() {

  PMC->PMC_PCER0 = (1U << ID_PIOB); // Enable peripheral clock
  SERVO_SIGNAL_BLOCK->PIO_CODR = SERVO_SIGNAL_PIN; // Set putput LOW
  SERVO_SIGNAL_BLOCK->PIO_OER = SERVO_SIGNAL_PIN;  // Enable output
  SERVO_SIGNAL_BLOCK->PIO_PER = SERVO_SIGNAL_PIN;  // Give PIO control of the pin

  for (int i = 0; i <= 20; i++) {
    SERVO_SIGNAL_BLOCK->PIO_SODR = SERVO_SIGNAL_PIN; // Set HIGH
    printf("Servo SIG: HIGH\r\n");
    time_spinFor(msecs(1000));

    SERVO_SIGNAL_BLOCK->PIO_CODR = SERVO_SIGNAL_PIN; // Set LOW
    printf("Servo SIG: LOW\r\n");
    time_spinFor(msecs(1000));
  }
}

void test_pwm() {
  servo_init();
  time_spinFor(msecs(1000));

  for (int i = 0; i <= 20; i++) {
    servo_go_right();
    printf("SERVO GO RIGHT // 20ms\r\n");
    time_spinFor(msecs(1000));

    servo_go_left();
    printf("SERVO GO LEFT // 10ms\r\n");
    time_spinFor(msecs(1000));
  }
  
}
