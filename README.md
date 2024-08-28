# R_OS
A simple monolothic OS for learning x86 barebones and how OSes work

### Version:
2.2 [Major changes in kernel startup!]

# Sources:
- [BIOS interrupts list](https://www.ctyme.com/intr/int.htm)
- [OSDev Wiki](https://wiki.osdev.org/Expanded_Main_Page)
- [Nanobyte OS Youtube](https://youtube.com/@nanobyte-dev?si=W3_2_LVOpSClpD6I)
- [Daedalus Community Youtube](https://youtube.com/@daedaluscommunity?si=xIHrJSebgPJretdI)

# Features:
- Not much, presently only bootbase.bin (first boot sector code) is finished and successfully reads and loads bootsetup.bin from
memory for execution and executes it
- bootsetup.bin, is in an intermediate step and it loads the kernel(4 sectors wide), enables A20, sets up GDT, and jumps to 32 bit protected mode kernel
- kernel.bin: Presently contains only a TTY driver for the project and writes some basic info and loads into kernel

# How to use?
## Requirements:
- An emulator: I am using `Qemu with target i386`, you can use Virtual Box but you will need to write your own
command to execute the binary at `build/main_floppy.img`(this is the OS!)
- `NASM` as the assembler
- A debugger: I am using `bochs` and would highly recommend that you do too
- And mostly knowledge about the topic to fix problems on your system

## Compile and Run:
- Run `make` to compile the OS and then use `make emu` to run on Qemu, also you can run with debugger with `make debug`

# Author:
Rouvik Maji [Gmail](mailto:majirouvik@gmail.com)