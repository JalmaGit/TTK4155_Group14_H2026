#ifndef UART_H
#define UART_H

#include <stdio.h>

void uart_init(unsigned int ubrr);

void uart_transmit(unsigned char data);
unsigned char uart_receive(void);

int uart_putchar(char c, FILE *stream);
int uart_getchar(FILE *stream);

#endif // UART_H
