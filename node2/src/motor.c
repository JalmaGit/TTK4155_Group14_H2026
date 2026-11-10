#include "motor.h"
#include "sam.h"
#include "serialize.h"
#include <stdbool.h>
#include <stdint.h>
#include "pid.h"
#include <math.h>

#define MOTOR_PWM_PIN PIO_PB12
#define MOTOR_DIR_PIN PIO_PC23
#define PWM_CLOCK_ENABLE_ID PMC_PCER1_PID36
#define MOTOR_PWM_ENABLE PWM_ENA_CHID0
#define MOTOR_PWM_DISABLE PWM_DIS_CHID0
#define MOTOR_PWM_PIN_ID PMC_PCER0_PID12
#define MOTOR_DIR_PIN_ID PMC_PCER0_PID13

#define MOTOR_SPEED_PERCENT 10 // 10%
#define MOTOR_DUTYCYCLE ((MOTOR_SPEED_PERCENT * 25U + 50U) / 100U)
#define CURRENT_POSITION TC2->TC_CHANNEL[0].TC_CV

volatile int32_t REFERENCE_POSITION = 0;

void encoder_init();
void motor_pwm_init();
void set_direction(bool direction);
bool get_direction(float move_value_float);
uint8_t get_dutycycle(uint8_t percent);
uint8_t set_speed(float u_float, uint8_t min_speed_percent, uint8_t max_speed_percent);

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

void motor_pwm_init() {
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
    PWM->PWM_CH_NUM[0].PWM_CPRD = 25; // Set PWM frequency to 40 kHz
    PWM->PWM_CH_NUM[0].PWM_CDTY = 0; // Set initial duty cycle to 0%
    
    // PWM pin
    PMC->PMC_PCER0 = MOTOR_PWM_PIN_ID; // Enable PIOB peripheral clock
    PIOB->PIO_ABSR |= MOTOR_PWM_PIN; // Select peripheral B (PWMH0)
    PIOB->PIO_PDR = MOTOR_PWM_PIN; // Give PWM peripheral control of PB12

    // Motor direction pin
    PMC->PMC_PCER0 = MOTOR_DIR_PIN_ID; // Enable PIOC peripheral clock
    PIOC->PIO_PER = MOTOR_DIR_PIN; // Enable GPIO control of PC23
    PIOC->PIO_OER = MOTOR_DIR_PIN; // Configure PC23 as output


    // Set standard direction left
    PIOC->PIO_CODR = MOTOR_DIR_PIN;  // Set direction left
    PWM->PWM_ENA = MOTOR_PWM_ENABLE; // Enable output (0% on pwm signal)
}

void motor_move_value(uint8_t move_value) {
    float move_value_float = byte_to_float(move_value, -1.0, 1.0);
    printf("Float value: %f\r\n", move_value_float);
    
    if ((move_value_float >= -0.5) && (move_value_float <= 0.5)) { // Don't move if joystick is idle
        
        PWM->PWM_CH_NUM[0].PWM_CDTYUPD = 0; // Set duty cycle to 0%
        return;
    }

    bool direction = get_direction(move_value_float);
    set_direction(direction);
    PWM->PWM_CH_NUM[0].PWM_CDTYUPD = MOTOR_DUTYCYCLE;
} 



bool get_direction(float move_value_float) {

    if (move_value_float >= 0){
        return 0; // left
    }
    else{
        return 1; // right
    }
}

void set_direction(bool direction){
    if (direction){
        PIOC->PIO_CODR = MOTOR_DIR_PIN;  // Set Direction Right
    }
    else {
        PIOC->PIO_SODR = MOTOR_DIR_PIN;  // Set Direction Left
    }
}


void motor_move_pid(uint8_t move_value){
    float float_temp = byte_to_float(move_value, -1, 1);

    REFERENCE_POSITION += (int32_t)(float_temp * 10);
    float u_float = controller_PID((float)(REFERENCE_POSITION),(float) (CURRENT_POSITION));
    
    set_direction(get_direction(u_float));
    uint8_t new_dutycycle = set_speed(u_float, 5, 15);
    PWM->PWM_CH_NUM[0].PWM_CDTYUPD = new_dutycycle;

}


uint8_t set_speed(float u_float, uint8_t min_speed_percent, uint8_t max_speed_percent) {

    if (min_speed_percent > 100) min_speed_percent = 100;
    if (max_speed_percent > 100) max_speed_percent = 100;

    if (min_speed_percent > max_speed_percent) {
        min_speed_percent = max_speed_percent;
    }

    float speed = fabsf(u_float);

    // Stop if the requested speed is essentially zero
    if (speed < 0.01f) {
        return 0;
    }

    // Enforce motor limits
    if (speed < min_speed_percent) {
        speed = min_speed_percent;
    }

    if (speed > max_speed_percent) {
        speed = max_speed_percent;
    }

    return get_dutycycle((uint8_t)(speed + 0.5f));
}

uint8_t get_dutycycle(uint8_t percent) {
    if (percent > 100) percent = 100;

    return (uint8_t)((percent * 25U + 50U) / 100U);
}


