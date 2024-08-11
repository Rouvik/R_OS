BUILD = ../../build
ASM ?= nasm

all:
	$(ASM) -f bin main.s -o $(BUILD)/bootbase.bin

clean:
	rm $(BUILD)/bootbase.bin