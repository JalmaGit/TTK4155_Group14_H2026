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
    DDRA |= (1 << PA7);

    USART_Init(MYUBRR);

    printf("UART ready!\n");

    while (1) {
        int c = getchar();
        status_light(1);
        putchar(c);
        _delay_ms(100);
        status_light(0);
    }
}

