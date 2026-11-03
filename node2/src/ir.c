#include "ir.h"
#include "sam.h"
#include "sam3x8e.h"

// As defined in uart.c, not sure where to find this actually
#define F_CPU 84000000
#define PRESCAL 41 // 20

// ~500-600 when not blocked
// ~0-100 when blocked
#define IR_THRESHOLD 300

void adc_init(void) {
  // Enable adc clock in the Power Managment Controller (PMC) before using the
  // ADC Controller
  // IR connected to A0 on Arduino which corresponds to pin PA16, which is AD7.
  // AD7 is an extra function and must be configured as in Section 43.5.3
  //
  // > The analog input pins can be multiplexed with PIO lines.
  // > In this case, the assignment of the ADC input is automatically done as
  // soon as the corresponding channel is enabled by writing the register
  // `ADC_CHER`.
  // > By default, after reset, the PIO line is configured as input
  // with its pull-up enabled and the ADC input is connected to the GND.
  //
  // AD0 is in peripheral X1
  // ADC has an instance ID of 37 in Peripheral Identifiers (9.1)
  //
  // Peripheral Clock Enable Register (write-only)
  PMC->PMC_PCER1 = 1u << (ID_ADC - 32); // PID37

  // 43.6.1
  // > The ADC uses the ADC Clock to perform conversions.
  // > Converting a single analog value to a 12-bit digital data requires
  // `Tracking Clock` cycles as defined in the field `TRACKTIM` of the `ADC Mode
  // Register` on page 1333 and selected in the `PRESCAL` field of the "Mode
  // Register" `ADC_MR`.
  // The ADC clock range is between MCK/2 (PRESCAL = 0)
  // and MCK/512 (PRESCAL = 255, 0xFF)
  //
  // PRESCAL: Prescaler Rate Selection
  // ADCClock = MCK / ((PRESCAL+1) * 2)
  //
  // TRACKTIM: Tracking Time
  // Tracking Time = (TRACKTIM + 1) * ADCClock periods
  // ADC->ADC_MR[1] = 255;
  //
  // 1 MHz conversion rate ?
  //
  // Single ended freerun mode easiest (no need to trigger)
  //
  // Set Prescaler
  ADC->ADC_MR = (PRESCAL << 8) // Set Prescaler
                | (1u << 7)    // Enable Freerun
                | (4u << 24)   // TRACKTIM
                | (8u << 16);  // STARTUP TIME;

  // Enable Freerun
  // ADC->ADC_MR = ENABLE << 7;

  // TRACKTIM (really don't know what to set this to)
  // ADC->ADC_MR = 4 << 24;

  // STARTUP TIME (really don't know what to set this to)
  // ADC->ADC_MR = 8 << 16;

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
