#include "adc.h"
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
