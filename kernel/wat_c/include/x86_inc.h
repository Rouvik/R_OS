#ifndef __x86INC__
#define __x86INC__

#include "stdint.h"
#include "../interrupts/idt.h"

// 64 bit division support
void _cdecl x86_div_u64(uint64_t divident, uint32_t divisor, uint64_t *quot, uint32_t *rem);

// Interrupts
void _cdecl x86_IDT_Load(IDTDesciptor_t *idtDescriptor);
void _cdecl x86_Death_After_Interrupt();
void _cdecl x86_sti();
void _cdecl x86_cli();

// IO
void _cdecl x86_outb(uint16_t portNumber, uint8_t data);
uint8_t _cdecl x86_inb(uint16_t portNumber);

#endif // __x86INC__
