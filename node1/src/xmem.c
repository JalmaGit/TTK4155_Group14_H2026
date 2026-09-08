#include "xmem.h"
#include "common.h"
#include <avr/io.h>

void xmem_init(void) {
  // Enable XMEM.
  //
  // `MCURC`: `MCU Control Register`
  //
  // `SRE`: `External SRAM/XMEM Enable`
  set_bit(MCUCR, SRE);

  // Release ports for other purposes such as JTAG.
  //
  // `SFIOR`: `Special Function IO Register`
  // `XMM0`, `XMM1`, `XMM2`: `External Memory High Mask`.
  //
  // `XMM2`=1, `XMM1`=0, `XMM0`=0 => `PC7-PC4` port released.
  // Releases the 4 most significant bits.
  set_bit(SFIOR, XMM2);
}

void xmem_write(uint8_t data, uint16_t addr) {
  volatile char *ext_mem = (char *)EXT_MEM_START;
  ext_mem[addr] = data;
}

uint8_t xmem_read(uint16_t addr) {
  volatile char *ext_mem = (char *)EXT_MEM_START;
  return ext_mem[addr];
}
