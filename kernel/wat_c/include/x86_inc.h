#ifndef __x86INC__
#define __x86INC__

#include "stdint.h"
#include "../interrupts/idt.h"

// 64 bit division support
void __cdecl x86_div_u64(uint64_t divident, uint32_t divisor, uint64_t *quot, uint32_t *rem);

// Interrupts
void __cdecl x86_IDT_Load(IDTDesciptor_t *idtDescriptor);
void __cdecl x86_Death_After_Interrupt();
void __cdecl x86_sti();
void __cdecl x86_cli();

// IO
void __cdecl x86_outb(uint16_t portNumber, uint8_t data);
uint8_t __cdecl x86_inb(uint16_t portNumber);

// VGA
uint16_t __cdecl x86_setVideoMode(uint16_t mode);

#endif // __x86INC__
