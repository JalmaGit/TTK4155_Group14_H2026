#include "motor.h"
#include "sam.h"
#include "serialize.h"
#include <stdbool.h>
#include <stdint.h>

#define MOTOR_PWM_PIN PIO_PB12
#define MOTOR_DIR_PIN PIO_PC23
#define PWM_CLOCK_ENABLE_ID PMC_PCER1_PID36
#define MOTOR_PWM_ENABLE PWM_ENA_CHID0
#define MOTOR_PWM_DISABLE PWM_DIS_CHID0
#define MOTOR_PWM_PIN_ID PMC_PCER0_PID12
#define MOTOR_DIR_PIN_ID PMC_PCER0_PID13

static uint16_t REFERENCE_POSITION = 0;

void encoder_init();
void motor_pwm_init();
void set_direction(bool direction);
bool get_direction(float move_value_float);

/*

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
    motor_pwm_init();
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

void motor_pwm_init(void) {
    /*
    To drive the motor you must
        * Apply a PWM signal to the MOTOR PWM Pin (any frequency below 200 kHz)
        * Apply a high or low signal to the MOTOR DIR pin

    The MOTOR PWM Pin is connected to Arduino pin D20 or ATSAM3X8E pin PB12. 
    PWM out on PB12 is set with alternative function B on PIOB and 
    frequency/dutycycle is set on PWM channel 0

    The MOTOR DIR pin is connected to Arduino pin D7, or ATSAM3X8E pin PC23.
    */


    // PWM configuration
    PMC->PMC_PCER1 = PWM_CLOCK_ENABLE_ID; // Enable PWM peripheral clock
    PWM->PWM_DIS = MOTOR_PWM_DISABLE; // Disable PWM channel 0
    PWM->PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(84); // Set clock A to 1 MHz (84 MHz / 84)
    PWM->PWM_CH_NUM[0].PWM_CMR = PWM_CMR_CPRE_CLKA | PWM_CMR_CPOL; // Clock A, left-aligned, inverted polarity
    PWM->PWM_CH_NUM[0].PWM_CPRD = 1000; // Set PWM frequency to 1 kHz
    PWM->PWM_CH_NUM[0].PWM_CDTY = 250; // Set initial duty cycle to 25%
    
    // PWM pin
    PMC->PMC_PCER0 = MOTOR_PWM_PIN_ID; // Enable PIOB peripheral clock
    PIOB->PIO_ABSR |= MOTOR_PWM_PIN; // Select peripheral B (PWMH0)
    PIOB->PIO_PDR = MOTOR_PWM_PIN; // Give PWM peripheral control of PB12

    // Motor direction pin
    PMC->PMC_PCER0 = MOTOR_DIR_PIN_ID; // Enable PIOC peripheral clock
    PIOC->PIO_PER = MOTOR_DIR_PIN; // Enable GPIO control of PC23
    PIOC->PIO_OER = MOTOR_DIR_PIN; // Configure PC23 as output
}

void motor_move_value(uint8_t move_value) {
    float move_value_float = byte_to_float(move_value, -1.0, 1.0);
    
    if ((move_value_float >= -0.2) && (move_value_float <= 0.2)) { // Don't move if joystick is idle
        
        PWM->PWM_DIS = MOTOR_PWM_DISABLE; // Disable PWM channel 0
        return;
    }

    bool direction = get_direction(move_value_float);
    set_direction(direction);
    PWM->PWM_ENA = MOTOR_PWM_ENABLE;
} 

bool get_direction(float move_value_float) {

    if (move_value_float >= 0){
        return 0; // left?
    }
    else{
        return 1; // right
    }
}

void set_direction(bool direction){
    if (direction){
        PIOC->PIO_SODR = MOTOR_DIR_PIN;  // Set PC23 HIGH
    }
    else {
        PIOC->PIO_CODR = MOTOR_DIR_PIN;  // Set PC23 LOW
    }
}
