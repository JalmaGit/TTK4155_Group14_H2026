#include <avr/io.h>
#include "common.h"
#include "spi.h"

#define MCP2515_CS_PIN PB4
#define FLAG_PIN PD2

#define MCP2515_CS 2 // IO_CS = 0 and DISP_CS = 1
#define READ_CMD 3
#define MCP_STATUS_INSTRUCTION 0b10100000

struct CANFrame{
    uint8_t ID; // Example says 11 bit
    uint8_t length;  // Example says 4 bit
    uint8_t data; // Example says 8 bit
};

void mcp2515_reset()   {
    uint8_t reset_cmd = 0b1100 0000;
    spi_transeive_byte(reset_cmd);
    // _delay_ms(20) might be necessary
}

void mcp2515_init(){ // Assumes that SPI_init has already been done
    
    set_bit(PORTB, MCP2515_CS_PIN); // Set PB4/CS as high, as CS inverted before we set it as an output pin
    set_bit(DDRA, MCP2515_CS_PIN); // Set PB4/CS to output
    set_bit(DDRD, FLAG_PIN); // Set FLAG_PIN as input

    mcp2515_reset();

    uint8_t status;
    mcp2515_read(MCP_STATUS_INSTRUCTION, &status);
    if ((status != ) || (MODE_MASK != )){
        printf (" MCP2515 is NOT in configuration mode after reset !\n" );
        return 1;
    }


    // TODO: bit timing
    // TODO: Mask and filter. Mask determines which ID bits are not DC. Filter determines the ID
    // TODO: Mode

}

uint8_t mcp2515_init_example () {
    uint8_t value;
    SPI_init (); // Initialize SPI
    mcp2515_reset(); // Send reset - command
    // Self-test
    mcp2515_read(MCP_CANSTAT, &value);
    if (( value & MODE_MASK ) != MODE_CONFIG ) {
        printf (" MCP2515 is NOT in configuration mode after reset !\n" );
        return 1;
    }
    // More initialization
    return 0;
}



uint8_t mcp2515_read(uint8_t address){
    // When the Interrupt pin is triggered, this function should be called
    uint8_t read_cmd = 0b00000011; // read
    uint8_t data;
    select_device(MCP2515_CS);
    spi_write(read_cmd);
    spi_write(address);
    data = spi_read();
    deselect_device(MCP2515_CS);
}


