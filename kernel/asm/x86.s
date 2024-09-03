bits 32

section _TEXT class=CODE
global _x86_div_u64

;
; _x86_div_u64 - Divides a 64 bit number by another 64 bit number
; Function signature: void _cdecl x86_div_u64(uint64_t divident, uint32_t divisor, uint64_t *quot, uint32_t *rem);
; Parameters:
;   - divident = ebp + 8   (8 bytes)
;   - divisor = ebp + 16    (4 bytes)
;   - quot = ebp + 20       (4 bytes long address)
;   - rem = ebp + 24        (4 bytes long address)
;
_x86_div_u64:
    push ebp
    mov ebp, esp

    push ebx

    ; divide upper 32 bits
    mov eax, [ebp + 12]          ; the upper 32 bits
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


