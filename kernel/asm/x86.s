bits 32

section _TEXT class=CODE
global _x86_div_u64
global _x86_IDT_Load
global _x86_Death_After_Interrupt
global _x86_sti
global _x86_cli
global _x86_outb
global _x86_inb

; x86 core ==================================================================

;
; _x86_div_u64 - Divides a 64 bit number by another 64 bit number
; Function signature: void _cdecl x86_div_u64(uint64_t divident, uint32_t divisor, uint64_t *quot, uint32_t *rem);
; Parameters:
;   - divident = ebp + 8    (8 bytes)
;   - divisor = ebp + 16    (4 bytes)
;   - quot = ebp + 20       (4 bytes long address)
;   - rem = ebp + 24        (4 bytes long address)
;
_x86_div_u64:
    push ebp
    mov ebp, esp

    push ebx

    ; divide upper 32 bits
    mov eax, [ebp + 12]         ; the upper 32 bits
    mov ecx, [ebp + 16]

    or ecx, ecx                 ; check for div by zero
    jz .div_by_zero

    xor edx, edx                ; edx = 0
    div ecx                     ; eax = quot, edx = rem
    
    mov ebx, [ebp + 20]         ; store upper 32 bits of quot
    mov [ebx + 4], eax

    ; divide lower 32 bits
    mov eax, [ebp + 8]          ; edx = old remainder [edx:eax division]
    div ecx

    ; save rest
    mov [ebx], eax
    mov ebx, [ebp + 24]
    mov [ebx], edx

    jmp .exit

.div_by_zero:
    ; set the quot and remainder both to -1 for div by zero error
    mov ebx, [ebp + 20]
    mov [ebx], dword -1
    mov ebx, [ebp + 24]
    mov [ebx], dword -1

.exit:
    pop ebx
    
    mov esp, ebp
    pop ebp
    ret

; IDT ==============================INTERRUPTS===============================

;
; _x86_IDT_Load - Loads the interrupt descriptor to the system IDTR
; Function signature: void _cdecl x86_IDT_Load(IDTDesciptor_t *idtDescriptor)
; Parameters:
;   - IDTDescriptor = ebp + 8 (4 byte address)
;
_x86_IDT_Load:
    push ebp
    mov ebp, esp

    push eax

    mov eax, [ebp + 8]  ; get the pointer to IDT Descriptor

    lidt [eax]          ; get the actual IDT Descriptor

    pop eax             ; restore regs and stack
    pop ebp
    ret

;
; _x86_Death_After_Interrupt - Clears interrupts and halts the CPU indefinetly
; Function Signature: x86_Death_After_Interrupt()
;
_x86_Death_After_Interrupt:
    cli                 ; clear interrupt flags to avoid re runs
    hlt
.hlt:                   ; hard halt just in case
    jmp .hlt

;
; _x86_sti - Enables interrupts for CPU
; Function signature: x86_sti()
;
_x86_sti:
    sti
    ret

;
; _x86_cli - Disables interrupts for CPU
; Function signature: x86_cli()
;
_x86_cli:
    cli
    ret

; IO =================================================================

;
; _x86_outb - Writes a byte to output port
; Function Signature: x86_outb(uint16_t portNumber, uint8_t data)
;
_x86_outb:
    mov dx, [esp + 4]
    mov al, [esp + 8]
    out dx, al
    ret

;
; _x86_inb - Reads a byte from input port
; Function Signature: x86_inb(uint16_t portNumber)
;
_x86_inb:
    mov dx, [esp + 4]
    xor eax, eax
    in al, dx
    ret
