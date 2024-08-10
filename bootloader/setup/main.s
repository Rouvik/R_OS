org 0x7E00
bits 16
%define ENDL 0x0D, 0x0A

begin:
    mov si, setup_msg
    call puts
    hlt

.hlt:
    jmp .hlt

;
; puts -- writes a string to bios output
; Parameters: ds:si = pointer to string
;
puts:
    push si
    push ax
    push bx

.loop:
    lodsb       ; mov a character to al
    or al, al   ; check if null
    jz .done

    mov ah, 0Eh
    mov bh, 0
    int 10h

    jmp .loop

.done:
    pop bx
    pop ax
    pop si
    ret

setup_msg: db "Setting up system...", ENDL, 0
