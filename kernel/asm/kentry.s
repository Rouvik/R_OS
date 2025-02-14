bits 32

segment _ENTRY class=CODE
global kentry
extern _kmain

;;;
;;; PAGING WORKS, BUT NOW TTY ADDRESS 0xB8000 IS NOT MAPPED TO A REAL ADDRESS, FIX THAT (IDENTITY MAP IT!)
;;;

kentry:
    ; 0x7E05 stores the GDT location
    ; we need to set the stack to start from there to use up the most memory
    mov esp, [0x7E05]       ; reset the stack
    mov ebp, esp

    mov dl, [0x7E04]        ; bootdrive number is stored in 0x7E04
    and edx, 0xFF           ; make sure dl is not contaminated
    push edx                ; push an int instead

    call get_current_location

    lea ecx, [_kmain]
    add ecx, eax

    call setup_paging       ; setup the page tables and enable paging

    call _kmain

    hlt
.hlt:
    jmp .hlt

get_current_location:
    pop esi
    push esi
    sub esi, get_current_location
    add esi, 0x15                   ; DEBUG -- error constant yet to figure
    mov eax, esi
    ret

;
; PAGING (32 bit protected mode) -----------------------------------
;

;
; setup_PDE_PTE - Populate the PDE table with appropriate values to allocate the kernel
;
setup_PDE_PTE:
    push eax
    push ebx
    push esi

    mov ebx, PageTableBuffer
    or ebx, 0x7                                 ; present, read/write, user
    mov [PageDirectoryBuffer + 4 * 192], ebx    ; write the PDE for 0x30000000

    mov esi, 6                                  ; setup the pages for higher kernel
    mov eax, 0x8000                             ; address 0x8000 to be mapped to 0x30000000
    or eax, 0x7                                 ; present, read/write, user
    mov ebx, PageTableBuffer

.loop:
    mov [ebx], eax
    add eax, 0x1000
    add ebx, 4
    dec esi
    or esi, esi
    jz .done
    jmp .loop

.done:
    ; setup identity paging for 0x7000 address space (bootloader data space)
    mov ebx, IdentityPageTableBuffer
    or ebx, 0x7                                 ; present, read/write, user
    mov [PageDirectoryBuffer], ebx              ; write the PDE for 0x00007000

    mov ebx, 0x7000                             ; address 0x7000 to be mapped to 0x00007000
    or ebx, 0x7                                 ; present, read/write, user
    mov [IdentityPageTableBuffer + 4 * 7], ebx
    add ebx, 0x1000                             ; address 0x8000 to be mapped to 0x00008000
    mov [IdentityPageTableBuffer + 4 * 8], ebx
    add ebx, 0x1000                             ; address 0x9000 to be mapped to 0x00009000
    mov [IdentityPageTableBuffer + 4 * 9], ebx
    add ebx, 0x1000                             ; address 0xa000 to be mapped to 0x0000a000
    mov [IdentityPageTableBuffer + 4 * 10], ebx
    add ebx, 0x1000                             ; address 0xb000 to be mapped to 0x0000b000
    mov [IdentityPageTableBuffer + 4 * 11], ebx
    
    pop esi
    pop ebx
    pop eax
    ret

;
; setup_paging - Setup the page tables and enable paging!
; WARNING - This code expects that the CPU is in 32 bit protected mode!!!
;
setup_paging:
    mov ecx, _kmain
    
    call setup_PDE_PTE                      ; populate the PDE and PTEs with appropriate initial page values
    mov eax, PageDirectoryBuffer
    mov cr3, eax

    mov ecx, _kmain

    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    ; lea ecx, [_kmain - 0x8000 + 0x30000000]
    lea ecx, [0xc86e - 0x8000 + 0x30000000]
    add esp, 4                              ; delete the return address from the stack

    push IdentityPageTableBuffer
    push PageTableBuffer
    push PageDirectoryBuffer                ; push all the buffer locations to be later accesses from Kernel Main

    call ecx                                ; call kmain with 32 bit address to its virtual page

    hlt                                     ; something went wrong if we reach here
.hlt:                                       ; hard halt
    jmp .hlt

struc PageDirectoryStruc
    .Present resb 1
    .Read_Write resb 1
    .User_Supervisor resb 1
    .WriteThrough resb 1
    .CacheDisabled resb 1
    .Accessed resb 1
    .Dirty resb 1                   ; or available for 4KB page directory
    .PageSize resb 1
    .Global resb 1                  ; or available for 4KB page directory
    .Available resb 3
    .PageTableBase resb 20
endstruc

struc PageTableStruc
    .Present resb 1
    .Read_Write resb 1
    .User_Supervisor resb 1
    .WriteThrough resb 1
    .CacheDisabled resb 1
    .Accessed resb 1
    .Dirty resb 1
    .PageAttributeTableIndex resb 1
    .Global resb 1
    .Available resb 3
    .PageBase resb 20
endstruc

; -------------- Page buffers ----------------
ALIGN(4096)
PageDirectoryBuffer:
    times 1024 dd 0

; alignment ignored here as expected to be aligned already
; ALIGN(4096)
PageTableBuffer:
    times 1024 dd 0
IdentityPageTableBuffer:
    times 16 dd 0

; -------------- Page Buffers End ------------------

