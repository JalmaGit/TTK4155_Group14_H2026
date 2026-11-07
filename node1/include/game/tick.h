#ifndef TICK_H
#define TICK_H

#include <stdint.h>

#define TICK_HZ 60
#define T0_PRESCALER 1024
#define T0_OCR (F_CPU / T0_PRESCALER / TICK_HZ - 1)

void tick_init(void);

// Returns missed ticks (e.g. "dropped frames")
uint8_t tick_wait(void);

void tick_stop(void);

#endif // TICK_H
