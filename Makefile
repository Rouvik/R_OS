# Targets
.PHONY: all mkdirs bootloader bootbase bootsetup kernel emu debug stub

# Stop printing the directories
MAKEFLAGS += --no-print-directory

SRC_BOOTLOADER := ./bootloader
SRC_KERNEL := ./kernel
BUILD := ./build

all: bootloader kernel
	dd if=/dev/zero of=$(BUILD)/main_floppy.img bs=512 count=2880
	dd if=$(BUILD)/bootbase.bin of=$(BUILD)/main_floppy.img conv=notrunc
	dd if=$(BUILD)/bootsetup.bin of=$(BUILD)/main_floppy.img obs=512 seek=1 conv=notrunc
	
# Boot loader compile section -------------------
bootloader: bootbase bootsetup

bootbase: $(BUILD)/bootbase.bin
bootsetup: $(BUILD)/bootsetup.bin

$(BUILD)/bootbase.bin: mkdirs
	$(MAKE) -C $(SRC_BOOTLOADER)/base BUILD=$(abspath $(BUILD))

$(BUILD)/bootsetup.bin: mkdirs
	$(MAKE) -C $(SRC_BOOTLOADER)/setup BUILD=$(abspath $(BUILD))

# Kernel compile section ------------------------

kernel: $(BUILD)/kernel.bin

$(BUILD)/kernel.bin: mkdirs
	$(MAKE) -C $(SRC_KERNEL) BUILD=$(abspath $(BUILD))

# Compile other programs in ./stub folder -------
stub:
	$(MAKE) -C ./stub

# directory gen section
mkdirs:
	mkdir -p $(BUILD)

# Clean the $(BUILD) but keep the folder
clean:
	rm -r $(BUILD)/*

# Qemu emulation command
emu:
	qemu-system-i386 $(BUILD)/main_floppy.img

# Bochs debug command
debug:
	bochsdbg