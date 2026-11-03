#include "can.h"
#include "sam.h"
#include "uart.h"
#include "servo.h"
#include <stdarg.h>
#include <stdio.h>
#include "tests/servo_tests.h"
#include "game/game.h"

/*
 * Remember to update the Makefile with the (relative) path to the uart.c file.
 * This starter code will not compile until the UART file has been included in
 the Makefile.
 * If you get somewhat cryptic errors referencing functions such as _sbrk,
 * _close_r, _write_r, _fstat etc, you have most likely not done that correctly.

 * If you get errors such as "arm-none-eabi-gcc: no such file", you may need to
 reinstall the arm gcc packages using
 * apt or your favorite package manager.
 */

int main() {
  SystemInit();

  WDT->WDT_MR = WDT_MR_WDDIS; // Disable Watchdog Timer

  /*PMC: Power Management Controller. Most peripherals are deactivated by
  default. See table 9-1 in datasheet. You will need to activate the clock for
  the peripherals you will be using.*/

  uart_init(SystemCoreClock, 9600); // SystemCoreClock = 84 000 000
  printf("UART initialized\n\r");

  CanInit can_values;
  can_values.phase1 = 6;
  can_values.phase2 = 7;
  can_values.propag = 2;
  can_values.sjw = 1;
  can_values.brp = 0;
  can_values.smp = 0;

  can_init(can_values, 1);
  servo_init();

  while (1) {
    // TODO: listen to pings
    // Read Joystick data
    game_start();
    
  }
}
