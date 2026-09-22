#include "adc.h"
#include "avr/io.h"
#include "common.h"
#include "xmem.h"
#include <stdbool.h>
#include <stdint.h>
#include <util/delay.h>

#define WR_PORT PORTD
#define WR_DIR_REG DDRD
#define WR_PIN PD6

#define RD_PORT PORTD
#define RD_DIR_REG DDRD
#define RD_PIN PD7

#define CLK_DIR_REG DDRD
#define CLK_PIN DDD5

#define ADC_MEM_START 0x1000

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

  // Set !WR Pin as Output
  set_bit(DDRD, WR_PIN);
  // Set !RD Pin as Output
  set_bit(DDRD, RD_PIN);

  configure_adc_clock();
}

void adc_mem_read() { volatile char *ext_mem = (char *)ADC_MEM_START; }

// TODO: Verify that CS is constant inbetween reads.
void adc_set_cs(void) { adc_mem_read(); }

void adc_clear_cs(void) { xmem_read(0x0); }

void adc_init_conversion(void) {
  // Set WR LOW
  clear_bit(WR_PORT, WR_PIN);
  // Wait min 100ns, max 2000ns
  _delay_us(1);
  // Set WR HIGH
  set_bit(WR_PORT, WR_PIN);
  // t_WBD max 220 ns => wait min 220ns
  _delay_us(1);
}

uint8_t adc_read_once(void) {
  // Set RD LOW to declare a read
  clear_bit(RD_PORT, RD_PIN);
  // Wait at least 100ns for stable data (t_DV)
  _delay_us(1);
  uint8_t value = xmem_read(0x1000);
  // Set RD HIGH to end the read
  set_bit(RD_PORT, RD_PIN);
  return value;
}

uint8_t adc_read(uint8_t channel) {
  // TODO: Verify that ADC data can be read from arbitrary address between
  // 0x1000 and 0x13FF
  //
  // Chip Select is controlled by the NAND gates bassed on the address output.
  //
  // Conversions are initiated with a pulse to the !WR pin,
  // and data is accessed from teh ADC's RAM with a pulse to the !RD pin.
  //
  // Timings:
  // !RD Low Pulse Width min 100ns
  // !RD High Pulse Width min 180ns
  // !WR to !RD Delay min 280ns
  // CLK to !WR Delay (Acquisition Time) 800ns IMPORTANT!
  // !RD to Data Valid min 100ns
  //
  // CLK Frequency min 0.5 MHz to max 5.0 MHz
  adc_set_cs();

  // Initiate a conversion (!WR Low min 100ns, max 2000ns)
  adc_init_conversion();

  // Wait at least 800ns
  // - or -
  // wait at least 220ns then wait for BUSY to go HIGH,
  // then wait another 50ns

  _delay_us(1); // Wait at least 800ns, instead of listening to busy

  // Wait at least 50ns after !BUSY goes HIGH beefore issuing a read if
  // waited for BUSY _delay_us(1);

  // Reads sequental data for each RD.
  // Starts at channel 0.
  // Is reset when !WR goes low (before a new query).
  // So if we only use channel 0 we don't need to think much of this.
  // But we need two channels for both axis?

  uint8_t value = 0;

  for (int ch = 0; ch < channel; ch++) {
    // Wait t_RDH min 180ns before reading again?
    _delay_us(1);
    value = adc_read_once();
  }

  // CS must be cleared ot reset the channel counter.
  adc_clear_cs();

  return value;
}
