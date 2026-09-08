#include <avr/io.h>
#include <stdio.h>

#include "uart.h"

void uart_init(unsigned int ubrr) {

  /* Set baud rate */
  UBRR1H = (unsigned char)(ubrr >> 8);
  UBRR1L = (unsigned char)ubrr;
  /* Enable receiver and transmitter */
  UCSR1B = (1 << RXEN1) | (1 << TXEN1);
  /* Set frame format: 8data, 2stop bit */
  UCSR1C = (1 << URSEL1) | (1 << USBS1) | (3 << UCSZ10);

  // configures printf to use uart
  fdevopen(uart_putchar, uart_getchar);
}

void uart_transmit(unsigned char data) {
  /* Wait for empty transmit buffer */
  while (!(UCSR1A & (1 << UDRE1)))
    ;

  /* Put data into buffer, sends the data */
  UDR1 = data;
}

unsigned char uart_receive(void) {
  /* Wait for data to be received */
  while (!(UCSR1A & (1 << RXC1)))
    ;

  /* Get and return received data from buffer */
  return UDR1;
}

// `printf` compatibility
int uart_putchar(char c, FILE *stream) {
  uart_transmit((unsigned char)c);
  return 0;
}

int uart_getchar(FILE *stream) { return uart_receive(); }
