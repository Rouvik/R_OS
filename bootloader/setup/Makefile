BUILD ?= ../build
ASM  ?= nasm

# all:
# 	@echo "Setup unimplemented skipping..."

all:
	$(ASM) -f bin main.s -o $(BUILD)/bootsetup_out.bin
	dd if=/dev/zero of=$(BUILD)/bootsetup.bin bs=512 count=2
	dd if=$(BUILD)/bootsetup_out.bin of=$(BUILD)/bootsetup.bin conv=notrunc
	rm $(BUILD)/bootsetup_out.bin