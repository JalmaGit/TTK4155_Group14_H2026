#include <stdint.h>
#include "board.h"
#include "sam3x8e.h"

#define PWM_PID PID36
#define PERIPHERAL_B 1
#define PERIPHERAL_A 0
#define SERVO_CHANNEL 1
#define SERVO_PERIOD PWM_CPRD
#define SERVO_DUTY_CYCLE PWM_CDTY
#define SERVO_UPDATE_DUTY_CYCLE PWM_CDTYUPD

/*
Servo period = 20ms = 50Hz
On-time 0.9 - 2.1ms
We need X = 32 and CPRD = 52 500 for T_period
and X = 32 and CDTY = 48562 for T_pulse 

Servo positions:
    0 degrees (left): 1.0ms
    90 degrees (down): 1.5ms
    180 degrees (right): 2.0ms

Need to use PIN44 and PIN45 on the shield ??? The website says to use pin D21?!?!

Important registers:
    PWM->PWM_CH_NUn.PWM_CMR PWM Channel Mode Register. Here CPRE decides the division of the MCK?
    PWM->PWM_CH_NUn.PWM_CPRD PWM Channel Period Register
    PWM->PWM_CH_NUn.PWM_CDTY PWM Channel Duty Cycle
*/

void servo_init(){
    // Use SAM3X pin PB13, Peripheral B, PWMH1
     
    PMC->PMC_PCER1 = (1U << (ID_PWM - 32)); // Enable peripheral clock signal
    PWM->PWM_CH_NUM[SERVO_CHANNEL].PWM_CMR = 5; // Sets X = 32 as the prescaler
    
    PWM->PWM_CH_NUM[SERVO_CHANNEL].SERVO_PERIOD = (0b1100110100010100); // Sets 52 500 as clock divider
    PWM->PWM_CH_NUM[SERVO_CHANNEL].SERVO_DUTY_CYCLE = (0b0000111101100010); // 3938, makes the duty cycle be 1.5ms

    PWM->PWM_CH_NUM[SERVO_CHANNEL].PWM_CMR |= PWM_CMR_CPOL; // Sets start of PWM to high
    
    SERVO_SIGNAL_BLOCK->PIO_ABSR |= SERVO_SIGNAL_PIN; // Select Peripheral B
    SERVO_SIGNAL_BLOCK->PIO_PDR = SERVO_SIGNAL_PIN;   // Give control to PWM
    PWM->PWM_ENA = (1U << SERVO_CHANNEL); // Enables PWM on servo channel
}

void servo_go_left() {
    PWM->PWM_CH_NUM[SERVO_CHANNEL].SERVO_UPDATE_DUTY_CYCLE = (0b0000101001000001); // 2625, makes the duty cycle be 1.0ms
}

void servo_go_right() {
    PWM->PWM_CH_NUM[SERVO_CHANNEL].SERVO_UPDATE_DUTY_CYCLE = (0b1010010000010); // 5250, makes the duty cycle be 2.0ms
}
