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
} _Packed ISR_Register_t;

typedef void (*ISR_HandlerF_t)(ISR_Register_t *regs); // typedef functions to handle the interrupts

ISR_HandlerF_t g_ISRHandlers[256] = {0};

void __cdecl x86_ISR_Handler(ISR_Register_t *registers);

void ISR_Initialise();

void ISR_RegisterHandler(int interrupt_number, ISR_HandlerF_t handler);

#endif // __ISR__
