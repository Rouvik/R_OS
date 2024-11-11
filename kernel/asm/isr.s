bits 32

section _TEXT class=CODE
extern _x86_ISR_Handler

; ISR Macros to make out lives easier
%macro ISR_NOERROR 1
global _x86_ISR%1
_x86_ISR%1:
    push 0                  ; dummy error code for interrupts with no error
    push %1                  ; push the interrupt number
    jmp isr_common_handler
%endmacro

%macro ISR_ERROR 1
global _x86_ISR%1
_x86_ISR%1:
    ; CPU pushes an error code
    push %1                  ; push the interrupt number
    jmp isr_common_handler
%endmacro

%include "./asm/isr_gen.inc.txt"

isr_common_handler:
    pusha                   ; push edi, esi, ebp, esp, ebx, ecx, eax

    xor eax, eax            ; push ds
    mov ax, ds
    push eax

    mov ax, 0x10            ; 32-bit data segment offset, from GDT, read in entry.s GDT
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    push esp                ; pass the stack frame to C for collection of data
    call _x86_ISR_Handler
    add esp, 4              ; get rid of the stack frame pushed

    pop eax                 ; restore old segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa                    ; restore all pusha values
    add esp, 8              ; get rid of interrupt number and error code
    iret                    ; the processor will pop: cs, eip, eflags, ss, esp