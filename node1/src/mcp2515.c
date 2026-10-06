#include <avr/io.h>
#include "common.h"
#include "spi.h"

#define MCP2515_CS_PIN PB4
#define FLAG_PIN PD2

#define MCP2515_CS 2 // IO_CS = 0 and DISP_CS = 1
#define READ_CMD 3


/*
TODO: Create Frame struct for each frame that needs 
to be sent. Should include ID, data-length, data
*/

void mcp2515_init(){
    // Set PB4 as high, as CS inverted before we set it as an output pin
    set_bit(PORTB, MCP2515_CS_PIN);
    set_bit(DDRA, MCP2515_CS_PIN);

    // Set FLAG_PIN as input
    set_bit(DDRD, FLAG_PIN);

    // TODO: bit timing
    // TODO: Mask and filter
    // TODO: Mode

}

uint8_t mcp2515_read(uint8_t address){
    // When the Interrupt pin is triggered, this function should be called

    // Put the READ-cmd into spi register

    uint8_t result;
    select_device(MCP2515_CS);
    spi_write
}

