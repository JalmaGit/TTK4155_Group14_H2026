#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#include "lights.h"

// System Oscillator clock frequency
#define FOSC F_CPU
// bits per second
#define BAUD 9600
// Table 70 in ATmega162 datasheet
#define MYUBRR FOSC / 16 / BAUD - 1

void USART_Init(unsigned int ubrr) {
  /* Set baud rate */
  UBRR1H = (unsigned char)(ubrr >> 8);
  UBRR1L = (unsigned char)ubrr;
  /* Enable receiver and transmitter */
  UCSR1B = (1 << RXEN1) | (1 << TXEN1);
  /* Set frame format: 8data, 2stop bit */
  UCSR1C = (1 << URSEL1) | (1 << USBS1) | (3 << UCSZ10);
}

void USART_Transmit(unsigned char data) {
  /* Wait for empty transmit buffer */
  while (!(UCSR1A & (1 << UDRE1)))
    ;

  /* Put data into buffer, sends the data */
  UDR1 = data;
}

unsigned char USART_Receive(void) {
  /* Wait for data to be received */
  while (!(UCSR1A & (1 << RXC1)))
    ;

  /* Get and return received data from buffer */
  return UDR1;
}

int uart_putchar(char c, FILE *stream) {
    USART_Transmit((unsigned char)c);
    return 0;
}

int uart_getchar(FILE *stream) {
    return USART_Receive();
}

FILE uart_stream = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);


int main() {
  // Pin A7 for Status LED

  // Set outputs
  DDRA = 0b10000000;

  USART_Init(MYUBRR);

  status_light(1);

  fdevopen(uart_putchar, uart_getchar);

  int i = 0;

  while (1) {
    i += 1;
    USART_Transmit(USART_Receive());
    printf("\nHello world!\n");
    printf("%d", i);
    /*USART_Transmit('a');*/
    status_light(1);
    //_delay_ms(500);

    USART_Transmit(USART_Receive());
    // USART_Transmit('b');
    status_light(0);
    //_delay_ms(500);
  }
}
