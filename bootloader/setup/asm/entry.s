bits 16

section _ENTRY class=CODE
extern _cstart_
global entry

jmp short entry
nop
; boot setup table ---------------------------
reserved_sectors: db 1      ; reserved sectors required to be loaded

entry:
    cli
    mov ax, 0          ; set segments to the beginning of program as they grow downward
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7E00               ; set stack pointer to 0
    mov bp, sp
    sti

    push dx                 ; dl is expected to contain the drive number from bootbase
    call _cstart_

    hlt

.hlt:           ; hard halt
    jmp .hlt