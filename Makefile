# Targets
.PHONY: all assembly mkdirs bootloader bootbase bootsetup kernel emu debug stub

# Stop printing the directories
# MAKEFLAGS += --no-print-directory

SRC_BOOTLOADER := ./bootloader
SRC_KERNEL := ./kernel
BUILD := ./build

all: assembly mkdirs
	@echo "All builds complete!"

assembly: bootloader kernel
	dd if=/dev/zero of=$(BUILD)/main_floppy.img bs=512 count=2880
	dd if=$(BUILD)/bootbase.bin of=$(BUILD)/main_floppy.img conv=notrunc
	dd if=$(BUILD)/bootsetup.bin of=$(BUILD)/main_floppy.img obs=512 seek=1 conv=notrunc
	dd if=$(BUILD)/kernel.bin of=$(BUILD)/main_floppy.img obs=512 seek=3 conv=notrunc
	
# Boot loader compile section -------------------
bootloader: bootbase bootsetup

bootbase: $(BUILD)/bootbase.bin
bootsetup: $(BUILD)/bootsetup.bin

$(BUILD)/bootbase.bin:
	$(MAKE) -C $(SRC_BOOTLOADER)/base

$(BUILD)/bootsetup.bin:
	$(MAKE) -C $(SRC_BOOTLOADER)/setup

# Kernel compile section ------------------------

kernel: $(BUILD)/kernel.bin

$(BUILD)/kernel.bin:
	$(MAKE) -C $(SRC_KERNEL)

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
	qemu-system-i386 -drive file=$(BUILD)/main_floppy.img,format=raw,if=ide

# Bochs debug command
debug:
	bochsdbg