#include "ui/menu.h"
#include "ui/menu_view.h"
#include "user_io.h"
#include <stddef.h>
#include <util/delay.h>

int menu_run(const char *title, const menu_item_t *items, int count,
             int *selected) {

  menu_clear();
  while (true) {
    menu_draw(title, items, count, *selected);

    JoyDir input = joy_read_dir();

    while (input != JOY_NEUTRAL) {
      _delay_ms(100);
      input = joy_read_dir();
    }

    while (input == JOY_NEUTRAL) {
      _delay_ms(100);
      input = joy_read_dir();
    }

    switch (input) {
    case JOY_UP:
      *selected -= 1;
      if (*selected < 0) {
        *selected = 0;
      }
      break;
    case JOY_DOWN:
      *selected += 1;
      if (*selected >= count) {
        *selected = count - 1;
      }
      break;
    case JOY_LEFT:
      return MENU_BACK;
    case JOY_RIGHT:
      return *selected;
    case JOY_NEUTRAL:
      break;
    }
  }
}
