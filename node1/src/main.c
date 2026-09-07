#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "lights.h"
#include "uart.h"

#include <avr/io.h>
#include <util/delay.h>

#include "lights.h"

int main(void)
{
    status_light_init();

    while (1) {
        status_light(1);
        _delay_ms(1000);

        status_light(0);
        _delay_ms(1000);
    }
}

