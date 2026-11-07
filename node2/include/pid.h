#ifndef PID_H
#define PID_H

#include <stdint.h>

void init_PID(float P, float I, float D);
float controller_PID(float target_value, float current_value);

#endif // !PID_H
