// OLED Driver
#ifndef OLED_H_
#define OLED_H_

#include <stdint.h>

void send_command(uint8_t command);

// Commands //

void display_on();

// Note: Automatic address increment on data commands

#endif // OLED_H_
