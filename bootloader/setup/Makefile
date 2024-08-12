BUILD = ../../build
ASM := nasm

.PHONY: all clean

all:
	nasm -f bin entry.s -o $(BUILD)/bootsetup_out.bin
	dd if=/dev/zero of=$(BUILD)/bootsetup.bin bs=512 count=2
	dd if=$(BUILD)/bootsetup_out.bin of=$(BUILD)/bootsetup.bin conv=notrunc
	rm $(BUILD)/bootsetup_out.bin

clean:
	rm $(BUILD)/bootsetup.bin
	rm $(BUILD)/bootsetup_out.bin