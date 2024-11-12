#ifndef __x86INC__
#define __x86INC__

#include "stdint.h"
#include "../interrupts/idt.h"

// 64 bit division support
void _cdecl x86_div_u64(uint64_t divident, uint32_t divisor, uint64_t *quot, uint32_t *rem);

// IDT
void _cdecl x86_IDT_Load(IDTDesciptor_t *idtDescriptor);
void _cdecl x86_Death_After_Interrupt();

#endif // __x86INC__
