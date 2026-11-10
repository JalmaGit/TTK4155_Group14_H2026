#include "pid.h"
#include "time.h"
#include <stdint.h>

typedef struct PID_Parameters {
  float P;
  float I;
  float D;
  uint64_t prev_time;
  float prev_error;
  float prev_integral;
  float windup_guard;
} PID_Parameters_t;

PID_Parameters_t PID_params;

void init_PID(float P, float I, float D) {
  PID_params.P = P;
  PID_params.I = I;
  PID_params.D = D;

  PID_params.prev_time = time_now();
  PID_params.prev_error = 0;
  PID_params.prev_integral = 0;
  PID_params.windup_guard = 1;
}

float windup_guard(float I) {
  if (I > PID_params.windup_guard) {
    I = PID_params.windup_guard;
  } else if (I < -PID_params.windup_guard) {
    I = -PID_params.windup_guard;
  }
  return I;
}

float controller_PID(float target_value, float current_value) {
  uint64_t now_time = time_now();
  float error = target_value - current_value;

  float delta_error = error - PID_params.prev_error;
  float delta_time = totalSeconds(now_time - PID_params.prev_time);

  float P = PID_params.P * error;
  float I = PID_params.prev_integral + PID_params.I * error * delta_time;
  float D = PID_params.D * delta_error / delta_time;

  I = windup_guard(I);

  PID_params.prev_integral = I;
  PID_params.prev_time = now_time;
  PID_params.prev_error = error;

  float U = P + I + D;
  return U;
}
