;   Bootsetup section, loaded after bootbase at 0x7E00 and this loads the enables legacy A20, loads kernel, sets up GDT for 32 bit protected mode and executes kernel
;   Copyright (C) 2024  Rouvik Maji
;    
;   This program is free software: you can redistribute it and/or modify
;   it under the terms of the GNU General Public License as published by
;   the Free Software Foundation, either version 3 of the License, or
;   (at your option) any later version.
;
;   This program is distributed in the hope that it will be useful,
;   but WITHOUT ANY WARRANTY; without even the implied warranty of
;   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;   GNU General Public License for more details.
;
;   You should have received a copy of the GNU General Public License
;   along with this program.  If not, see <https://www.gnu.org/licenses/>.
;
;   Contact: majirouvik@gmail.com

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
bootdrive_number: db 0      ; the boot drive number to be stored here again
gdt_location: dd 0          ; store the dynamic GDT location here
memtable_size: db 0         ; store the Memory Map size here (the number of elements)

entry:
    mov [bootdrive_number], dl  ; save the bootdrive number again

    mov si, setup_memory_msg
    call ttyWrite

    cli
    mov ax, 0                           ; set segments to the beginning of program as they grow downward
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7E00                      ; set stack pointer to 0
    mov bp, sp
    sti

    mov si, setup_a20_msg
    call ttyWrite

    call EnableA20                      ; Enable the A20 line for larger addressing
    
    mov si, setup_gen_memory_table_msg  ; Get the system available memory map
    call ttyWrite

    mov di, 0x8200                      ; Write to location 0x8200
    call gen_memory_table

    mov si, setup_kernel_msg
    call ttyWrite

    call LoadKernel

    mov si, setup_gdt_msg
    call ttyWrite

    call LoadGDT                        ; Loads the Global Descriptor Table

    mov esp, [0x7E05]                   ; reset the stack
    mov ebp, esp

    cli                                 ; DEBUG -- DISABLING INTERRUPT AS IT CAN CAUSE FUNNY TRIPLE ERRORS AND UNCONTROLLED FULL SYSTEM RESET

    ; set pr (protected mode) flag in cr0 (bit 0)
    mov eax, cr0
    or al, 1
    mov cr0, eax

    jmp dword 08h:0x8300                ; the kernel sits at 0x8300

    hlt

.hlt:                                   ; hard halt
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

;
; disk_reset -- Resets the disk
; Paramaters:
;   - dl = drive number
;
disk_reset:
    push ax

    stc
    mov ah, 00h
    int 13h
    jc .error

    pop ax
    ret

.error:
    mov si, disk_reset_error_msg
    call ttyWrite
    hlt

.hlt:
    jmp .hlt

;
; disk_read
; Parameters:
;   - al = number of sectors to read
;   - ch = low eight bits of cylinder number
;   - cl [0-5] = sector number
;   - cl [6-7] = high 2 bits of cylinder
;   - dh = head number
;   - dl = drive number
;   es:bx = data buffer
;
disk_read:
    push di

    mov di, 3       ; number of retries
.loop:
    pusha
    call disk_reset ; reset the disk
    mov ah, 02h
    stc
    int 13h
    popa
    jnc .done
    
    dec di
    or di, di
    jnz .loop

.error:
    mov si, disk_read_error_msg
    call ttyWrite
    hlt
    
.hlt:       ; hard halt
    jmp .hlt

.done:
    pop di
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

;
; gen_memory_table - Finds and writes a memory table for available extended memory in 0x8200
; Parameters:
;   - di = buffer pointer for result
;   - ecx = Size of buffer for result
; Returns:
;   - ecx = Actual length returned in bytes
;
gen_memory_table:
    push eax
    push ebx
    push ecx
    push edx
    push si

    mov edx, 534D4150h      ; SMAP key
    xor ebx, ebx            ; ebx = 0
    clc                     ; clear carry before call
    xor si, si              ; si = 0

.loop:
    mov eax, 0000E820h      ; BIOS Get System Memory Map command
    mov ecx, 24             ; Read in 24 byte chunks ACPI 3.0
    int 15h
    jc .error
    inc si                  ; increment si per valid list element
    add di, 24              ; Increment the location to copy more
    or ebx, ebx
    jnz .loop

.done:
    mov [memtable_size], si ; Write the number of elements to memtable_size
    mov si, setup_gen_memory_table_success_msg
    call ttyWrite

    pop si
    pop edx
    pop ecx
    pop ebx
    pop eax
    ret

.error:
    cmp ah, 86h
    jz .unsupported
    mov si, setup_gen_memory_table_error_msg
    call ttyWrite

.unsupported:
    mov si, setup_gen_memory_table_unsupported_error_msg
    call ttyWrite

.hlt:           ; hard halt
    jmp .hlt

; KERNEL -----------------------------------
LoadKernel:
    push bx

    mov al, 6                   ; read 6 sectors
    mov ch, 0                   ; from cylinder 0
    mov cl, 3                   ; sector 3
    add cl, [reserved_sectors]  ; and the reserved sectors later
    mov dl, [bootdrive_number]  ; from bootdrive
    mov dh, 0                   ; with head = 0
    mov bx, 0x8300              ; to location 0x8300
    call disk_read

    mov si, setup_kernel_loaded_msg
    call ttyWrite

    pop bx
    ret

; TTY messages
setup_memory_msg: db "Setting up system segments...", ENDL, 0
setup_a20_msg: db "Enabling legacy A20...", ENDL, 0
setup_gen_memory_table_msg: db "Detecting system memory map...", ENDL, 0
setup_gen_memory_table_error_msg: db "Failed to load system memory map", ENDL, 0
setup_gen_memory_table_unsupported_error_msg: db "Error, memory mapping is unsupported in this system", ENDL, 0
setup_gen_memory_table_success_msg: db "System memory map loaded at 0x8200", ENDL, 0
setup_gdt_msg: db "Setting up GDT and memory partitions, preparing to enter Protected mode...", ENDL, 0
setup_pmode_msg: db "Successfully entered protected mode!", ENDL, 0
setup_kernel_msg: db "Loading kernel...", ENDL, 0
setup_kernel_loaded_msg: db "Loading additional kernel sectors...this might take some time", ENDL, 0

disk_read_error_msg: db "Failed to read disk after 3 tries...please reset and boot again", ENDL, 0
disk_reset_error_msg: db "Failed to reset disk...please reboot", ENDL, 0

; GDT ---------------------------------------
LoadGDT:
    lgdt [g_GDTDesc]
    mov dword [gdt_location], g_GDT
    ret

; Global descriptor table -- https://wiki.osdev.org/Global_Descriptor_Table
g_GDT:
    dq 0            ; first entry should be zero

    ; 32-bit code segment
    dw 0FFFFh       ; limit(0 - 15) for full 32bit range
    dw 0            ; base (0 - 15)
    db 0            ; base(16 - 23)
    db 10011011b    ; access(present, ring 0, code seg, executable, dir up, readable)
    db 11001111b    ; granularity = 4k pages, 32bit protected mode, no long mode
    db 0            ; base (24 - 31)

    ; 32-bit data segment
    dw 0FFFFh       ; limit(0 - 15) for full 32bit range
    dw 0            ; base (0 - 15)
    db 0            ; base(16 - 23)
    db 10010011b    ; access(present, ring 0, data seg, NON executable, dir up, readable)
    db 11001111b    ; granularity = 4k pages, 32bit protected mode, no long mode
    db 0            ; base (24 - 31)

    ; 16-bit code segment
    dw 0FFFFh       ; limit(0 - 15) for full 16bit range [note granularity = 0]
    dw 0            ; base (0 - 15)
    db 0            ; base(16 - 23)
    db 10011011b    ; access(present, ring 0, code seg, executable, dir up, readable)
    db 00001111b    ; granularity = 1b pages, 16bit protected mode, no long mode
    db 0            ; base (24 - 31)

    ; 16-bit data segment
    dw 0FFFFh       ; limit(0 - 15) for full 16bit range [note granularity = 0]
    dw 0            ; base (0 - 15)
    db 0            ; base(16 - 23)
    db 10010011b    ; access(present, ring 0, data seg, NON executable, dir up, readable)
    db 00001111b    ; granularity = 1b pages, 16bit protected mode, no long mode
    db 0            ; base (24 - 31)

; GDT load entry
g_GDTDesc:
    dw g_GDTDesc - g_GDT - 1    ; size
    dd g_GDT                    ; offset