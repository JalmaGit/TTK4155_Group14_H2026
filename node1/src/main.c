#include <avr/io.h>
#include <util/delay.h>

#include "lights.h"
#include "uart.h"

int main() {
  // // Pin A7 for Status LED

  // // Set outputs
  // DDRA = 0b10000000;

  // USART_Init(MYUBRR);

  // status_light(1);

  // while (1) {
  //   USART_Transmit(USART_Receive());
  //   /*USART_Transmit('a');*/
  //   status_light(1);
  //   //_delay_ms(500);

  //   USART_Transmit(USART_Receive());
  //   // USART_Transmit('b');
  //   status_light(0);
  //   //_delay_ms(500);


  // ######################## Sindre Prøver C-kode ##########################

  DDRA |= (1 << PA7); // Tells the Data Direction Register (DDRA) to set PA7 as an output

  USART_Init(MYUBRR);

  while (1) {
    PORTA |= (1 << PA7); // Sets the output on PA7 to HIGH
    _delay_ms(500); // Wait for 500 milliseconds

    PORTA &= ~(1 << PA7); // Sets the output on PA7 to LOW
    _delay_ms(500); // Wait for 500 milliseconds
  }
}

