#include <avr/io.h>
#include <stdio.h>

#define UART_H
#define FOSC F_CPU
#define BAUD 9600
#define MYUBRR (FOSC / 16 / BAUD - 1)

void USART_Init(unsigned int ubrr);
void USART_Transmit(unsigned char data);
unsigned char USART_Receive(void);
int uart_putchar(char c, FILE *stream);
int uart_getchar(FILE *stream);