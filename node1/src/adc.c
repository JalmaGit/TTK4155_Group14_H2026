#include "adc.h"
#include "avr/io.h"
#include "common.h"
#include "xmem.h"
#include <stdbool.h>
#include <stdint.h>
#include <util/delay.h>

#define CLK_DIR_REG DDRD
#define CLK_PIN PD5

void configure_adc_clock(void) {
  // Set Pin PD5 to be output
  set_bit(CLK_DIR_REG, CLK_PIN);

  // Toggle OC1A/OC1B on Compare Match
  set_bit(TCCR1A, COM1A0);
  clear_bit(TCCR1A, COM1A1);

  // Configure Waveform Generation Mode.
  // CTC (TOP = OCRnA)
  // WGM10 = 0
  // WGM11 = 0
  // WGM12 = 1
  // WGM13 = 0
  clear_bit(TCCR1A, WGM10);
  clear_bit(TCCR1A, WGM11);
  set_bit(TCCR1B, WGM12);
  clear_bit(TCCR1B, WGM13);

  // Prescaler set to 1
  clear_bit(TCCR1B, CS12);
  clear_bit(TCCR1B, CS11);
  set_bit(TCCR1B, CS10);

  // f_OC1A_PFC_PWM = f_clk_IO/(2*N*TOP)
  // N = 1, 8, 64, 256, 1024
  //
  // 4 915 200 / (2*1*2) = 1 228 800
  //
  // due to it toggles at each period, the freq is halfed,
  // and therefor OCR1A is set to 1 instead of 2.
  // OCR1A
  OCR1A = 1;
}

void adc_init(void) {
  // The ADC may be wired for "output-only" operation:
  // - Connect MODE to LOW (GND?) -> 4-channel, single-ended conversion,
  // Note Capacitors for stable reference (fig 8a)

  configure_adc_clock();
}

uint8_t adc_mem_read() {
  volatile char *ext_mem = (char *)ADC_MEM_START;
  return ext_mem[0];
}

void adc_mem_write(uint8_t data) {
  volatile char *ext_mem = (char *)ADC_MEM_START;
  ext_mem[0] = data;
}

void adc_init_conversion(void) { adc_mem_write(0); }

uint8_t adc_read_once(void) { return adc_mem_read(); }

uint8_t adc_read(uint8_t channel) {
  // Chip Select is controlled by the NAND gates bassed on the address output.

  // Conversions are initiated with a pulse to the !WR pin,
  // and data is accessed from teh ADC's RAM with a pulse to the !RD pin.
  // CLK Frequency min 0.5 MHz to max 5.0 MHz

  adc_init_conversion();

  // Wait for conversion
  _delay_us(5);

  // Reads sequental data for each RD.
  // Starts at channel 0.
  // Is reset when !WR goes low (before a new query).

  uint8_t value = 0;

  for (int ch = 0; ch <= channel; ch++) {
    // Wait t_RDH min 180ns before reading again
    _delay_us(1);
    value = adc_read_once();
  }

  return value;
}
