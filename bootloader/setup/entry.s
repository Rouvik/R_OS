org 0x7E00
bits 16

%define ENDL 0x0D, 0x0A

; PORT CONSTANTS for PS2 keyboard controller
%define KbdControllerDataPort 0x60
%define KbdControllerCommandPort 0x64

jmp short entry
nop
; boot setup table ---------------------------
reserved_sectors: db 1      ; reserved sectors required to be loaded

entry:
    mov si, setup_memory_msg
    call ttyWrite

    cli
    mov ax, 0          ; set segments to the beginning of program as they grow downward
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7E00      ; set stack pointer to 0
    mov bp, sp
    sti

    mov si, setup_a20_msg
    call ttyWrite

    call EnableA20      ; Enable the A20 line for larger addressing    

    mov si, setup_debug_msg
    call ttyWrite

    hlt

.hlt:           ; hard halt
    jmp .hlt

;
; ttyWrite -- Writes a string to TTY
; Parameters:
;   - si = location of string
;
ttyWrite:
    push ax
    push bx

    mov ah, 0Eh
    mov bh, 0

.loop:
    lodsb
    or al, al
    jz .done

    int 10h
    jmp .loop

.done:
    pop bx
    pop ax
    ret

; Here we attempt to enable the A20 line to read higher addresses from CPU
; To do this the keyboard controller is used to disable the A20 line legacy behaviour
; Keyboard Controller docs - https://github.com/qb40/keyboard-controller

; Enables the A20 line from 8042 Keyboard controller
EnableA20:
    cli
    
    call WaitA20Input
    mov al, 0xAD                        ; disable the controller
    out KbdControllerCommandPort, al

    call WaitA20Input
    mov al, 0xD0                        ; Read from controller output port
    out KbdControllerCommandPort, al

    call WaitA20Output
    in al, KbdControllerDataPort        ; Move data read from data port to al
    push ax
    
    call WaitA20Input
    mov al, 0xD1                        ; Write output port command
    out KbdControllerCommandPort, al

    call WaitA20Input
    pop ax
    or al, 2
    out KbdControllerDataPort, al       ; Actually write to Kbd Controller

    call WaitA20Input
    mov al, 0xAE                        ; Enable controller
    out KbdControllerCommandPort, al

    call WaitA20Input
    sti
    ret

; wait until input buffer is empty
WaitA20Input:
    in al, KbdControllerCommandPort
    test al, 2
    jnz WaitA20Input
    ret

; wait until output buffer has data
WaitA20Output:
    in al, KbdControllerCommandPort
    test al, 1
    jz WaitA20Output
    ret

; GDT ---------------------------------------
LoadGDT:
    ret

setup_memory_msg: db "Setting up system segments...", ENDL, 0
setup_a20_msg: db "Enabling legacy A20...", ENDL, 0
setup_debug_msg: db "The process stops here for now... :(", ENDL, 0