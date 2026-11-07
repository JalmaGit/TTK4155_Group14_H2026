# List all source files to be compiled; separate with space
SOURCE_FILES := src/main.c \
								src/uart.c \
								src/xmem.c \
								src/adc.c \
								src/user_io.c \
								src/display/oled.c \
								src/display/ssd1309.c \
								src/spi.c \
								src/game/game.c \
								src/game/game_ui.c \
								src/game/tick.c \
								src/can/mcp2515_low_level.c \
								src/can/mcp2515_high_level.c \
								src/can/config.c \
								src/ui/menu.c \
								src/ui/menu_view.c \
								src/tests/sram_test.c \
								src/tests/led_test.c \
			 					src/tests/can_bus_test.c

include ../common/Makefile

# Set this flag to "yes" (no quotes) to use JTAG; otherwise ISP (SPI) is used
PROGRAM_WITH_JTAG := yes

# Feel free to ignore anything below this line
PROGRAMMER := atmelice_isp
ifeq ($(PROGRAM_WITH_JTAG), yes)
	PROGRAMMER := atmelice
endif

BUILD_DIR := build
TARGET_CPU := atmega162
TARGET_DEVICE := m162

CC := avr-gcc
# custom addtions:
# - add `include` folder
# - add `#define F_CPU` to make it available everywhere sleep is used
# - add `-I$(COMMON_INCLUDE)` to include Mafefile from common/ 
CFLAGS := -O -std=c11 -mmcu=$(TARGET_CPU) -ggdb -Iinclude -I$(COMMON_INCLUDE) -DF_CPU=4915200

OBJECT_FILES = $(SOURCE_FILES:%.c=$(BUILD_DIR)/%.o) \
							 $(COMMON_SOURCES:$(COMMON_DIR)%.c=$(BUILD_DIR)/common/%.o)

.DEFAULT_GOAL := $(BUILD_DIR)/main.hex

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/common/%.o: $(COMMON_DIR)%.c | $(BUILD_DIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/main.hex: $(OBJECT_FILES) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(OBJECT_FILES) -o $(BUILD_DIR)/a.out
	avr-objcopy -j .text -j .data -O ihex $(BUILD_DIR)/a.out $(BUILD_DIR)/main.hex

.PHONY: flash
flash: $(BUILD_DIR)/main.hex
	avrdude -p $(TARGET_DEVICE) -c $(PROGRAMMER) -U flash:w:$(BUILD_DIR)/main.hex:i

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: erase
erase:
	avrdude -p $(TARGET_DEVICE) -c $(PROGRAMMER) -e
	
.PHONY: debug
debug:
	if pgrep avarice; then pkill avarice; fi
	avrdude -p $(TARGET_DEVICE) -c $(PROGRAMMER) -U flash:w:$(BUILD_DIR)/main.hex:i
	x-terminal-emulator -e avarice --edbg --ignore-intr :4242
	sleep 2 avr-gdb -tui -iex "target remote localhost:4242" $(BUILD_DIR)/a.out
	killall -s 9 avarice	

.PHONY: compiledb
compiledb:
	bear -- $(MAKE) clean $(BUILD_DIR)/main.hex

.PHONY: fmt
fmt:
	clang-format -i $(SOURCE_FILES) $(wildcard include/*.h)
