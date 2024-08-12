all:
	@echo "Kernel Unimplemented...skipping"

# BUILD = ../../build
# ASM  ?= nasm

# CC16 ?= wcc
# LD16 ?= wlink
# CFLAGS16 ?= -4 -d3 -s -wx -ms -zl -zq -za99

# C_SRC_DIR = ./wat_c
# ASM_SRC_DIR = ./asm

# C_TARGETS = $(wildcard $(C_SRC_DIR)/*.c)
# ASM_TARGETS = $(wildcard $(ASM_SRC_DIR)/*.s)

# C_OBJECTS = $(patsubst $(C_SRC_DIR)/%.c, $(BUILD)/bootsetup/%.obj, $(C_TARGETS))
# ASM_OBJECTS = $(patsubst $(ASM_SRC_DIR)/%.s, $(BUILD)/bootsetup/%.obj, $(ASM_TARGETS))

# .PHONY: all build mkdirs clean

# all: build mkdirs
# 	dd if=/dev/zero of=$(BUILD)/bootsetup.bin bs=512 count=2
# 	dd if=$(BUILD)/bootsetup_out.bin of=$(BUILD)/bootsetup.bin conv=notrunc
# 	rm $(BUILD)/bootsetup_out.bin
	
# build: $(C_OBJECTS) $(ASM_OBJECTS)
# 	$(LD16) NAME $(BUILD)/bootsetup_out.bin FILE \{ $(ASM_OBJECTS) $(C_OBJECTS) \} OPTION MAP=$(BUILD)/bootsetup/bootsetup.map @linker.lnk

# # C build rule
# $(BUILD)/bootsetup/%.obj: $(C_SRC_DIR)/%.c mkdirs
# 	$(CC16) $(CFLAGS16) -fo=$@ $<

# # Assembly build rule
# $(BUILD)/bootsetup/%.obj: $(ASM_SRC_DIR)/%.s mkdirs
# 	$(ASM) -f obj -o $@ $<

# # Make sure the build directories exist before building
# mkdirs:
# 	mkdir -p $(BUILD)/bootsetup

# clean:
# 	rm $(BUILD)/bootsetup.bin
# 	rm $(BUILD)/bootsetup/* -r