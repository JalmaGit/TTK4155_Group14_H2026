// This file defines the menus/submenues along with their actions.

#ifndef MENU_ITEMS_H_
#define MENU_ITEMS_H_

#include "display/oled.h"
#include "game/game.h"
#include "tests/can_bus_test.h"
#include "tests/led_test.h"
#include "ui/menu.h"

typedef enum {
  TEST_MENU_OLED,
  TEST_MENU_LED,
  TEST_MENU_CAN,
  TEST_MENU_CAN_NORMAL,
} test_menu_action;

static void menu_tests(void) {
  static const menu_item_t items[] = {
      {"OLED Test", TEST_MENU_OLED},
      {"LED Test", TEST_MENU_LED},
      {"CAN Test Loopback", TEST_MENU_CAN},
      {"CAN Test Normal", TEST_MENU_CAN_NORMAL},
  };

  int selected = 0;

  for (;;) {
    switch (menu_run("Tests", items, 4, &selected)) {
    case TEST_MENU_OLED:
      oled_test();
      break;
    case TEST_MENU_LED:
      led_test();
      break;
    case TEST_MENU_CAN:
      can_bus_test();
      break;
    case TEST_MENU_CAN_NORMAL:
      can_bus_test_normal();
      break;
    case MENU_BACK:
      return;
    }
  }
}

typedef enum {
  MAIN_MENU_START,
  MAIN_MENU_TESTS,
} main_menu_action;

void menu_main(void) {
  static const menu_item_t items[] = {
      {"Start", MAIN_MENU_START},
      {"Tests", MAIN_MENU_TESTS},
  };

  int selected = 0;

  for (;;) {
    switch (menu_run("Main", items, 2, &selected)) {
    case MAIN_MENU_START:
      game_start();
      break;
    case MAIN_MENU_TESTS:
      menu_tests();
      break;
    }
  }
}

#endif // MENU_ITEMS_H_
