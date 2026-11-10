#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>

typedef struct {
  int min;
  int width;
} MotorCalibrationResult;

void motor_init();
void motor_calibrate(void);
void motor_move_value(uint8_t move_value);
void motor_move_pid(uint8_t move_value);

int32_t get_motor_position_raw(void);

// Returns a value between 0 and 1, where 0.5 is the center.
float get_motor_position_normalized(void);

#endif // MOTOR_H
