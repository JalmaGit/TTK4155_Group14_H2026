#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>

void servo_init();
void servo_move(uint16_t joystick_value);
void servo_go_left();
void servo_go_right();
void servo_center();


#endif //SERVO_H