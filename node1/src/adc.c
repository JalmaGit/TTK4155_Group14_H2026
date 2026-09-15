#include "adc.h"
#include "avr/io.h"
#include "common.h"
#include "xmem.h"

void adc_init(void) {
  // TODO: Figure out how data is to be read + timing details.
  // TODO: Configure ADC Clock
  // TODO: Connect Clock to ADC
  //
  // Connect MODE to LOW (GND?) -> 4-channel, single-ended conversion,
  // Note Capacitors for stable reference (fig 8a)
  //
  // Conversions are initiated with a pulse to the !WR pin,
  // and data is accessed from teh ADC's RAM with a pulse to the !RD pin.
  //
  // The ADC may be wired for "output-only" operation. (?)
  //
  // Remember use of volatile when using memory-mapped io.
  //
  // Configure a timer/counter (CTC or PWM) -> square waves within 0.5 MHz
  // to 5.0 MHz.
  // Use PD4 or PD5 for this.
  //
  // From Alternate Functions of Port D:
  // PD4:
  //  - TOSC1 (Timer Oscillator Pin 1)
  //  - XCK0 (USART0 External Clock Input/Output)
  //  - **OC3A** (Timer/Counter3 Output Compare A Match Output)
  // PD5:
  //  - TOSC2 (Timer Oscillator Pin 2)
  //  - **OC1A** (Timer/Counter1 Output Compare A Match Output)
  //    - DDD5 set (one) needed to serve output function
  //    - Also the output pin for the PWM mode timer function

  // TODO: Verify
  // Set Pin to be output
  set_bit(DDRD, DDD5);

  // Toggle OC1A/OC1B on Compare Match
  set_bit(TCCR1A, COM1A0);
  clear_bit(TCCR1A, COM1A1);

  // Configure Waveform Generation Mode.
  // CTC (TOP = OCRnA)
  // WGM13 = 0
  // WGM12 = 1
  // WGM11 = 0
  // WGM10 = 0
  clear_bit(TCCR1B, WGM13);
  set_bit(TCCR1B, WGM12);
  clear_bit(TCCR1A, WGM11);
  clear_bit(TCCR1A, WGM10);

  // Prescaler set to 1
  clear_bit(TCCR1B, CS12);
  clear_bit(TCCR1B, CS11);
  set_bit(TCCR1B, CS10);

  // OCR1A
  OCR1A = 1;

  // f_OC1A_PFC_PWM = f_clk_IO/(2*N*TOP)
  // N = 1, 8, 64, 256, 1024
  //
  // 4 915 200 / (2*1*2) = 1 228 800
  //
  // due to it toggles at each period, the freq is halfed,
  // and therefor OCR1A is set to 1 instead of 2.
  //
  //
  //
  // freq 1 MHz ->
  // F_CPU = 4 915 200

  // 4 915 200 / (2*1*3) = 819200
}

// TODO: Verify this is 8 bits.
uint8_t adc_read(uint8_t channel) {
  // TODO: Verify that ADC data can be read from arbitrary address between
  // 0x1000 and 0x13FF
  //
  // Timings:
  // !RD Low Pulse Width min 100ns
  // !RD High Pulse Width min 180ns
  // !WR to !RD Delay min 280ns
  // CLK to !WR Delay (Acquisition Time) 800ns IMPORTANT!
  // !RD to Data Valid min 100ns
  //
  // CLK Frequency min 0.5 MHz to max 5.0 MHz
  //

  // TODO: Initiate a conversion (!WR Low min 100ns, max 2000ns)
  //
  // Wait at least 800ns (interrupt or delay?) (NOT NEEDED IF QUERYING BUSY)
  //
  // TODO: Wait for !BUSY to go to HIGH (conversion complete) before reading
  //
  // However, figure 3 seems like !BUSY is HIGH both before and after
  // conversion... can it be trusted alone, or is waiting necessary?
  // t_WBD = !WR to !BUSY Low delay 220ns, so I guess 220ns wait first...
  //
  // TODO: Wait at least 50ns after !BUSY goes HIGH beefore issuing a read
  // TODO: Decleare that we are going to read (set RD?)
  // TODO: Wait at least 100ns for stable data (t_DV)
  // TODO: Read data
  //
  // Reads sequental data for each RD.
  // Starts at channel 0.
  // Is reset when !WR goes low (before a new query).
  // So if we only use channel 0 we don't need to think much of this.
  // But we need two channels for both axis?

  return xmem_read(0x1000);
}
