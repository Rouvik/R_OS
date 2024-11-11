#ifndef __ISR__
#define __ISR__

#include "../include/stdint.h"
#include "../include/gdt_inc.h"
#include "../include/stdio.h"
#include "idt.h"

typedef struct
{
    // in reverse order as pushed
    uint32_t ds;
    uint32_t edi, esi, ebp, kern_esp, ebx, edx, ecx, eax;
    uint32_t interrupt_number, error;
    uint32_t eip, cs, eflags, esp, ss;
} _Packed ISR_Registers_t;

void _cdecl x86_ISR_Handler(ISR_Registers_t *registers);

void ISR_Initialise();

#endif // __ISR__
