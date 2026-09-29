#include "display/ssd1309.h"
#include "common.h"
#include "spi.h"
#include <avr/io.h>
#include <stdint.h>

// Data/Command selector for OLED
#define DC_DIR_REG DDRD
#define DC_PIN PD0
#define DC_PORT PORTD

// SPI Ports
#define IO_CS PB1
#define DISP_CS PB0
#define PORT_SPI PORTB

void ssd1309_init(void) {
  // Set D!C Pin to be Output
  set_bit(DC_DIR_REG, PD0);

  // Segment remap (Flip Horizontally)
  send_command(0xA1);
  // Scan Direction (Flip Vertically)
  send_command(0xC8);
  // Output Enable
  send_command(0xAF);
}

void send_command(uint8_t command) {
  // Set D/C# LOW
  clear_bit(DC_PORT, DC_PIN);
  spi_master_transceive(command, &PORT_SPI, DISP_CS);
}

void send_data(uint8_t data) {
  // Set D/C# HIGH
  set_bit(DC_PORT, DC_PIN);

  spi_master_transceive(data, &PORT_SPI, DISP_CS);
}

// Commands //

void display_on(void) {
  send_command(0x81);
  send_command(0x7F);
}

void addressing_mode_horizontal(void) {
  send_command(0x20);
  send_command(0);
}

void addressing_mode_vertical(void) {
  send_command(0x20);
  send_command(1);
}

void addressing_mode_page(void) {
  send_command(0x20);
  send_command(2);
}

// This command is only for horizontal or vertical addressing mode.
void set_column_address(uint8_t start, uint8_t end) {
  send_command(0x21);
  send_command(start);
  send_command(end);
}

// This command is only for horizontal or vertical addressing mode.
void set_page_address(uint8_t start, uint8_t end) {
  send_command(0x22);
  send_command(start);
  send_command(end);
}
