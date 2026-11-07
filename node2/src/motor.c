#include "motor.h"
#include "sam.h"

void encoder_init();
/*
    To drive the motor you must
        * Apply a PWM signal to hte MOTOR PWM Pin (any frequency below 200 kHz)
        * Apply a high or low signal to the MOTOR DIR pin

    The MOTOR PWM Pin is connected to Arduino pin D20 or ATSAM3X8E pin PB12. 
    PWM out on PB12 is set with alternative function B on PIOB and 
    frequency/dutycycle is set on PWM channel 0

    The MOTOR DIR pin is connected to Arduino pin D7, or ATSAM3X8E pin PC23.

    The current-limiter for the motor is set to 1.9A with a resistor of 33k.
    The NOR-gate ???? outputs directs the PWM signal to one input, and sets the
    other to low. ????
    The result is FAST CURRENCT DECAY MODE or "coasting" behaviour
    

    It seems like I have to set up a timer (TC2) to keep the count of the position 
    (relative to the startup). The first thing in the exercise is to set this up and 
    see if the encode can read the relative position of the motor.
    */

void motor_init() {
    encoder_init();
    // motor_pwm_init();
}

void encoder_init() {
    /*
    Use TC2 which is connected to PC25/26 
    Quadrature decoder mode requires setting the block mode register, which will 
    change the operating mode of the entire TC2 instance. You must also set XC0 
    as the selected clock for channel 0.
    */

    PMC->PMC_PCER0 = (1u << ID_PIOC);
    PMC->PMC_PCER1 = (1u << (ID_TC6 - 32)); // Make Power Management Controller (PMC) enable clock

    PIOC->PIO_ABSR |= (1u << 25) | (1u << 26);
    PIOC->PIO_PDR = (1u << 25) | (1u << 26); // Hand control of the pins from PIO to the peripheral
    // PC25 = TIOA6, PC26 = TIOB6

    TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKDIS; // Stop the counter while configuring it
    TC2->TC_BMR = TC_BMR_QDEN | TC_BMR_POSEN; // Enable quadrature decoding with position measurement
    TC2->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_XC0; // Select XC0 as the channel's clock;
    TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG; // Enable the counter and reset/start it

}