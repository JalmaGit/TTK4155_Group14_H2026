#include <stdint.h>
#include <stdio.h>
#include "sam.h"
#include "time.h"
/* 
Exercise 3.6.5 1) Sets the servo header signal pin to high and verify with oscilloscope
*/

void test_servo_signal(){
    
    PIOB->PIO_CODR = PIO_PB13;  // Set putput LOW
    PIOB->PIO_OER  = PIO_PB13;  // Enable output
    PIOB->PIO_PER  = PIO_PB13;  // Give PIO control of the pin

    for (int i = 0;i<=20;i++){
        PIOB->PIO_SODR = PIO_PB13; // Set HIGH
        printf("Servo SIG: HIGH\r\n");
        time_spinFor(msecs(1000));

        PIOB->PIO_CODR = PIO_PB13; // Set LOW
        printf("Servo SIG: LOW\r\n");
        time_spinFor(msecs(1000));

    }
}