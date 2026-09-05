#include "uart.h"
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