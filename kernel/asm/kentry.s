bits 32

segment _ENTRY class=CODE
global kentry
extern _kmain

kentry:
    ; 0x7E05 stores the GDT location
    ; we need to set the stack to start from there to use up the most memory
    mov esp, [0x7E05]       ; reset the stack
    mov ebp, esp

    mov dl, [0x7E04]        ; bootdrive number is stored in 0x7E04

    push dx
    call _kmain

    hlt

.hlt:
    jmp .hlt
