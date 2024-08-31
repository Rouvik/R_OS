BUILD = ../build
ASM  ?= nasm

CC386 ?= wcc386
LD386 ?= wlink

# Flag description: 4(r/s) = 386 instruction with 486 timing, full debug, no stack check, warn all, flat memory, no obj link, quiet, C99
CFLAGS386 ?= -4s -d3 -s -wx -mf -zl -zq -za99

C_SRC_DIR = ./wat_c
ASM_SRC_DIR = ./asm

C_TARGETS = $(wildcard $(C_SRC_DIR)/*.c) $(wildcard $(C_SRC_DIR)/drivers/*.c)
ASM_TARGETS = $(wildcard $(ASM_SRC_DIR)/*.s)

C_OBJECTS = $(patsubst $(C_SRC_DIR)/%.c, $(BUILD)/kernel/%.obj, $(C_TARGETS))
ASM_OBJECTS = $(patsubst $(ASM_SRC_DIR)/%.s, $(BUILD)/kernel/%.obj, $(ASM_TARGETS))

.PHONY: all build mkdirs clean

all: build mkdirs
	dd if=/dev/zero of=$(BUILD)/kernel.bin bs=512 count=6
	dd if=$(BUILD)/kernel_out.bin of=$(BUILD)/kernel.bin conv=notrunc
	rm $(BUILD)/kernel_out.bin

build: $(C_OBJECTS) $(ASM_OBJECTS)
	$(LD386) NAME $(BUILD)/kernel_out.bin FILE \{ $(ASM_OBJECTS) $(C_OBJECTS) \} OPTION MAP=$(BUILD)/kernel/kernel.map @linker.lnk

# C build rule
$(BUILD)/kernel/%.obj: $(C_SRC_DIR)/%.c mkdirs
	$(CC386) $(CFLAGS386) -fo=$@ $<

# Assembly build rule
$(BUILD)/kernel/%.obj: $(ASM_SRC_DIR)/%.s mkdirs
	$(ASM) -f win32 -o $@ $<

# Make sure the build directories exist before building
mkdirs:
	mkdir -p $(BUILD)/kernel
	mkdir -p $(BUILD)/kernel/drivers

clean:
	rm $(BUILD)/kernel.bin
	rm $(BUILD)/kernel/* -r