org 0x7E00
bits 16

%define ENDL 0x0D, 0x0A
%define BOOT_INFO 0x7C03        ; pointer to location where bootbase.bin stores critical info

jmp short begin
nop
; setup sector table -------------------------
reserved_sectors: db 1

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
