#ifndef IR_H
#define IR_H

#include <stdbool.h>

void adc_init(void);

int read_adc(void);

bool is_ir_blocked(void);

#endif // IR_H
