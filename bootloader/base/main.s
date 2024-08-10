org 0x7C00
bits 16

%define ENDL 0x0D, 0x0A

jmp short start
nop
drive_number:                   db 0    ; memory location to store drive number
setup_sectors:                  db 2    ; number of sectors taken by setup.bin

start:
    mov [drive_number], dl      ; save the drive number

    mov ax, 0                   ; set es and ds to 0
    mov es, ax  
    mov ds, ax  

    ; setup stack   
    mov ss, ax  
    mov sp, 0x7C00              ; start at the end of boot sector

    ; set segments to 0000:7C00 instead of 7C00:0000 as per some BIOSes
    push es
    push word .after
    retf

.after:
    mov si, init_msg            ; display init message
    call puts
    
    mov al, [setup_sectors]     ; read setup_sectors number of sectors
    mov ch, 0                   ; from cylinder 0
    mov cl, 2                   ; from sector 2
    mov dh, 0                   ; from head 0
    mov dl, [drive_number]      ; use base dl drive number
    mov bx, buffer
    call disk_read

    mov si, success_msg         ; display success message
    call puts

    jmp buffer                  ; jump to buffer, setup.bin begins here
    hlt
    jmp globl_halt

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
    call puts
    hlt
    jmp globl_halt

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
    call puts
    hlt
    jmp globl_halt

.done:
    pop di
    ret

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

;
; globl_halt -- hard halts the system in case hlt fails
;
globl_halt:
    jmp globl_halt

init_msg: db "Welcome to RBL - Rouvik's Boot Loader -- 11/08/2024 Licensed GNU GPL", ENDL, "Reading setup.bin for initialisation", ENDL, ENDL, 0
success_msg: db "Loaded setup successfully! to 0x7E00", ENDL, ENDL, 0

disk_read_error_msg: db "Failed to read disk after 3 tries, giving up... please reset and boot", ENDL, 0
disk_reset_error_msg: db "Failed to reset disk please restart", ENDL, 0

times 510-($-$$) db 0
dw 0AA55h

buffer:
