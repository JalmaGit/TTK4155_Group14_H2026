#include "ir.h"
#include "sam.h"
#include "sam3x8e.h"

#define F_CPU 84000000
#define PRESCAL 41 // 20

// ~500-600 when not blocked
// ~0-100 when blocked
#define IR_THRESHOLD 300

void adc_init(void) {
  // Peripheral Clock Enable Register (write-only)
  // Enable ADC clock
  PMC->PMC_PCER1 = 1u << (ID_ADC - 32); // PID37

  // The ADC clock range is between MCK/2 (PRESCAL = 0)
  // and MCK/512 (PRESCAL = 255, 0xFF)
  //
  // PRESCAL: Prescaler Rate Selection
  // ADCClock = MCK / ((PRESCAL+1) * 2)
  //
  // TRACKTIM: Tracking Time
  // Tracking Time = (TRACKTIM + 1) * ADCClock periods
  //
  // Single ended freerun mode easiest (no need to trigger)
  ADC->ADC_MR =
      (PRESCAL << 8) // Set Prescaler (this value happens to work, but was
                     // chosen arbitrarily)
      | (1u << 7)    // Enable Freerun
      | (4u << 24)   // TRACKTIM (this value works, but was chosen arbitrarily)
      | (8u << 16);  // STARTUP TIME (this value happens to work, but was chosen
                     // arbitrarily);

  // Enable channel AD7 (which maps to A0 on the Arduino) (write-only)
  ADC->ADC_CHER = ADC_CHER_CH7;

  // Start conversion
  ADC->ADC_CR = 1u << 1;
}

int read_adc(void) {
  while (!(ADC->ADC_ISR & ADC_ISR_DRDY)) {
  }

  return ADC->ADC_CDR[7];
}

bool is_ir_blocked(void) { return read_adc() < IR_THRESHOLD; }
