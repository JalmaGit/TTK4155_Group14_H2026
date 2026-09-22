#include "display/oled.h"
#include "display/fonts.h"
#include "display/ssd1309.h"
#include <util/delay.h>

void oled_init(void) { display_on(); }

void oled_write_char8(char c) {
  for (int i = 0; i < 8; i++) {
    send_data(get_glyph8_byte(c, i));
  }
}
void oled_write_char5(char c) {
  for (int i = 0; i < 5; i++) {
    send_data(get_glyph5_byte(c, i));
  }
}

void oled_write_char4(char c) {
  for (int i = 0; i < 4; i++) {
    send_data(get_glyph4_byte(c, i));
  }
}

void oled_test(void) {
  // Draw "Test"

  addressing_mode_horizontal();
  set_column_address(0, 127);
  set_page_address(0, 1);

  while (1) {
    oled_write_char5('T');
    oled_write_char5('e');
    oled_write_char5('s');
    oled_write_char5('t');
    oled_write_char5('!');
    oled_write_char5(' ');
    _delay_ms(2000);
  }
}
