// Constant definitions for the node1 board.

#ifndef BOARD_H_
#define BOARD_H_

#include <avr/io.h>

// --- ADC -- //
#define ADC_CLK_DDR DDRD
#define ADC_CLK_BIT PD5

// --- Chip Select --- //
#define CS_PORT PORTB
#define CS_DDR DDRB
#define CS_DISP_BIT PB0
#define CS_IO_PIN PB1

// CS IDs, used in `select_device()`
enum { CS_ID_IO, CS_ID_OLED };

// CLOCK //
// System Oscillator clock frequency
#define FOSC F_CPU
// bits per second
#define BAUD 9600

// --- External Memory --- //
#define MEM_EXT_START 0x1400
#define MEM_ADC_START 0x1000

// --- UART -- //
// Table 70 in ATmega162 datasheet
#define MY_UBRR FOSC / 16 / BAUD - 1

// --- SPI --- //
#define SPI_PORT PORTB
#define SPI_DDR DDRB
#define SPI_MOSI_BIT PB5
#define SPI_MISO_BIT PB6
#define SPI_SCK_BIT PB7

// --- SSD1309 --- //
#define DISP_DC_DDR DDRD
#define DISP_DC_BIT PD0
#define DISP_DC_PORT PORTD

#endif // BOARD_H_
