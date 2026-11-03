#include "servo.h"
#include <stdint.h>
#include "board.h"
#include "sam3x8e.h"
#include "serialize.h"

#define PWM_PID PID36
#define PERIPHERAL_B 1
#define PERIPHERAL_A 0
#define SERVO_CHANNEL 1
#define SERVO_PERIOD PWM_CPRD
#define SERVO_DUTY_CYCLE PWM_CDTY
#define SERVO_UPDATE_DUTY_CYCLE PWM_CDTYUPD
#define DUTYCYCLE_MAX_LEFT 2625     // Makes the duty cycle be 1.0ms
#define DUTYCYCLE_CENTER 3938       // Makes the duty cycle be 1.5ms
#define DUTYCYCLE_MAX_RIGHT 5250    // Makes the duty cycle be 2.0ms
#define CLOCK_DIVIDER 52500         // Makes the pwm clock go 

/*
Servo period = 20ms = 50Hz
On-time 0.9 - 2.1ms

Servo positions:
    0 degrees (left): 1.0ms
    90 degrees (down): 1.5ms
    180 degrees (right): 2.0ms
*/

void servo_init(){
    // Use SAM3X pin PB13, Peripheral B, PWMH1
     
    PMC->PMC_PCER1 = (1U << (ID_PWM - 32)); // Enable peripheral clock signal
    PWM->PWM_CH_NUM[SERVO_CHANNEL].PWM_CMR = 5; // Sets X = 32 as the prescaler
    
    PWM->PWM_CH_NUM[SERVO_CHANNEL].SERVO_PERIOD = CLOCK_DIVIDER; // Sets 52 500 as clock divider
    PWM->PWM_CH_NUM[SERVO_CHANNEL].SERVO_DUTY_CYCLE = DUTYCYCLE_CENTER; // 3938, makes the duty cycle be 1.5ms

    PWM->PWM_CH_NUM[SERVO_CHANNEL].PWM_CMR |= PWM_CMR_CPOL; // Sets start of PWM to high
    
    SERVO_SIGNAL_BLOCK->PIO_ABSR |= SERVO_SIGNAL_PIN; // Select Peripheral B
    SERVO_SIGNAL_BLOCK->PIO_PDR = SERVO_SIGNAL_PIN;   // Give control to PWM
    PWM->PWM_ENA = (1U << SERVO_CHANNEL); // Enables PWM on servo channel
}

float get_ms_from_joystick(uint8_t joystick){
    return byte_to_float(joystick, (float) (1), (float) (2));
}

uint16_t get_dutycycle_from_ms(float ms){
    if (ms < 1.0f) ms = 1.0f;
    if (ms > 2.0f) ms = 2.0f;

    return (uint16_t)(2625.0f + (ms - 1.0f) * 2625.0f + 0.5f);
}

uint16_t clamp(uint16_t dutycycle){
    if ( dutycycle < DUTYCYCLE_MAX_LEFT) { return DUTYCYCLE_MAX_LEFT; }
    else if ( dutycycle > DUTYCYCLE_MAX_RIGHT ) { return DUTYCYCLE_MAX_RIGHT; }
    else { return dutycycle; }
}

void servo_move(uint16_t joystick_uint16_t) {
    float ms = get_ms_from_joystick(joystick_uint16_t);
    uint16_t dutycycle = get_dutycycle_from_ms(ms);
    
    PWM->PWM_CH_NUM[SERVO_CHANNEL].SERVO_UPDATE_DUTY_CYCLE = clamp(dutycycle);
}


void servo_go_left() {
    PWM->PWM_CH_NUM[SERVO_CHANNEL].SERVO_UPDATE_DUTY_CYCLE = DUTYCYCLE_MAX_LEFT; // Left = 2625, makes the duty cycle be 1.0ms
}

void servo_center(){
        PWM->PWM_CH_NUM[SERVO_CHANNEL].SERVO_UPDATE_DUTY_CYCLE = DUTYCYCLE_CENTER; // Center = 3938, makes the duty cycle be 1.5ms
}

void servo_go_right() {
    PWM->PWM_CH_NUM[SERVO_CHANNEL].SERVO_UPDATE_DUTY_CYCLE = DUTYCYCLE_MAX_RIGHT; // 5250, makes the duty cycle be 2.0ms
}
