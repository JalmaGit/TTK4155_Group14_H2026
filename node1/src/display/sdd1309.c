#include "display/sdd1309.h"
#include <stdint.h>

void send_command(uint8_t command) {
  // TODO: Implement
  // Set D/C# LOW (?)
}

void display_on() { send_command(0x81); }
