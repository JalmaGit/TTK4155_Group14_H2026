#include "display/oled.h"
#include "ui/menu.h"
#include <stdint.h>

void menu_clear() { oled_clear_screen(); }

void menu_draw(const char *title, const menu_item_t *items, uint8_t count,
               uint8_t selected) {

  oled_pos(0, 0);
  oled_write_str_large(title);

  for (int i = 0; i < count; i++) {
    oled_pos(i + 1, 0);
    if (i == selected) {
      oled_write_char_medium('>');
    } else {
      oled_write_char_medium(' ');
    }
    oled_write_str_medium(items[i].label);
  }
}
