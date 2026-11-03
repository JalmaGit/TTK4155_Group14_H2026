LDSCRIPT = sam/flash.ld
BOOTUP = sam/sam3x/source/exceptions.c \
		sam/sam3x/source/startup_sam3x.c \
		sam/sam3x/source/system_sam3x.c
MCUTYPE = __SAM3X8E__
# List all source files (i.e. .c files) to be compiled; separate with whitespace.
# In particular, remember to add the UART and CAN starter files.
# Use relative file paths and a backslash before newline.
# Do NOT use a backslash after the last file.
TEST_FILES := $(wildcard src/tests/*.c)

SOURCE_FILES = \
	$(BOOTUP) \
	src/main.c \
	src/uart.c \
	src/can.c \
	src/time.c \
	src/servo.c\
	src/game/game.c\
	../common/src/serialize.c

SOURCE_FILES += $(TEST_FILES)
# Feel free to ignore anything below this line
BUILD_DIR := build
DEP_DIR := $(BUILD_DIR)/deps

OBJS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SOURCE_FILES))
DEPS := $(patsubst %.c,$(DEP_DIR)/%.d,$(SOURCE_FILES))
#Tools
CC:=arm-none-eabi-gcc
LD:=arm-none-eabi-gcc
AR:=arm-none-eabi-ar
AS:=arm-none-eabi-as

ELF=$(BUILD_DIR)/main.elf

LDFLAGS:= -T$(LDSCRIPT) -mthumb -mcpu=cortex-m3 -Wl,--gc-sections
CFLAGS:= -mcpu=cortex-m3 -mthumb -g -std=c11 -MMD
CFLAGS+= -I sam -I sam/sam3x/include -I sam/sam3x/source -I sam/cmsis -I .
CFLAGS+= -D $(MCUTYPE)
CFLAGS += -Iinclude
CFLAGS += -I../common/include
# CFLAGS += -Iinclude -I../lib



.DEFAULT_GOAL := $(ELF)
# compile and generate dependancy info
$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@) $(dir $(DEP_DIR)/$*.d)
	$(CC) -c $(CFLAGS) -MF $(DEP_DIR)/$*.d -MT $@ $< -o $@

$(BUILD_DIR)/%.o : %.s | $(BUILD_DIR)
	$(AS) $< -o $@

$(ELF) : $(OBJS) | $(BUILD_DIR)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)


.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: debug
debug: $(ELF)
	if pgr openocd; then killall -s 9 openocd; fi
	x-terminal-emulator -e openocd -f sam/openocd.cfg -c "program $(ELF) verify"
	sleep 5
	arm-eabi-gdb -tui -iex "target extended-remote localhost:3333" $(ELF)
	killall -s 9 openocd	

.PHONY: flash
flash: $(ELF)
#	#./sam/write_bin.sh sam/openocd.cfg $(BUILD_DIR)/main.elf
#	if pgr openocd; then killall -s 9 openocd; fi
	openocd -f sam/openocd.cfg -c "program ${BUILD_DIR}/main.elf verify reset exit"

.PHONY: compiledb
compiledb:
	bear -- $(MAKE) clean $(BUILD_DIR)/main.elf

.PHONY: fmt
fmt:
	find src include -type f \( -name '*.c' -o -name '*.h' \) -exec clang-format -i {} +

# pull in dependencies
-include $(DEPS)

