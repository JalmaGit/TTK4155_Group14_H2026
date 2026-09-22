#include "display/sdd1309.h"
#include <stdint.h>

void send_command(uint8_t command) {
  // TODO: Implement
  // Set D/C# LOW (?)
}

// Commands //

void display_on(void) { send_command(0x81); }

void addressing_mode_horizontal(void) {
  send_command(0x20);
  send_command(0);
}

void addressing_mode_vertical(void) {
  send_command(0x20);
  send_command(1);
}

void addressing_mode_page(void) {
  send_command(0x20);
  send_command(2);
}

// This command is only for horizontal or vertical addressing mode.
void set_column_address(uint8_t start, uint8_t end) {
  send_command(0x21);
  send_command(start);
  send_command(end);
}

// This command is only for horizontal or vertical addressing mode.
void set_page_address(uint8_t start, uint8_t end) {
  send_command(0x22);
  send_command(start);
  send_command(end);
}
