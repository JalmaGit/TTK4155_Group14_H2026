#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>

#define MOTOR_P 2
#define MOTOR_I 0.5
#define MOTOR_D 0.05

typedef struct {
  int min;
  int width;
} MotorCalibrationResult;

void motor_init();
void motor_calibrate(void);
void motor_move_value(uint8_t move_value);
void motor_move_pid(float joy_in);

int32_t get_motor_position_raw(void);

// Returns a value between 0 and 1, where 0.5 is the center.
float get_motor_position_normalized(void);

#endif // MOTOR_H
