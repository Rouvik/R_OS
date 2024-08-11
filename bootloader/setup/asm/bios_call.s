bits 16

section _TEXT class=CODE
extern _bios_writeCharTTY

;
; putc -- Prints a char to BIOS tty output
; sig: putc(char ch, uint8_t page)
_bios_writeCharTTY:
    push bp
    mov bp, sp

    push bx

    ; [bp + 0] = old call frame
    ; [bp + 2] = return address (small model so 16 bit address)
    ; [bp + 4] = char ch
    ; [bp + 6] = page
    mov ah, 0Eh
    mov al, [bp + 4]
    mov bh, [bp + 6]
    int 10h

    pop bx

    mov sp, bp
    pop bp
    ret