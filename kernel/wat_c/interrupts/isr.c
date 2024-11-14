#include "isr.h"

static const char * const ISR_Exception_Messages[] = {
    "Error: Division By Zero",
    "Error: Debug",
    "Error: Non Maskable Interrupt",
    "Trap: Breakpoint",
    "Trap: Oveflow",
    "Error: Bound Range Exceeded",
    "Error: Invalid Opcode",
    "Error: Device Not Available",
    "Critical Error: Double Fault",
    "Error: Coprocessor Segment Overrun",
    "Error: Invalid TSS",
    "Error: Segment Not Present",
    "Error: Stack Segment Fault",
    "Error: General Protection Fault",
    "Error: Page Fault",
    "Interrupt: Reserved (0xD)",
    "Error: x87 Floating Point Exception",
    "Error: Alignment Check",
    "Error: Machine Check",
    "Error: SIMD Floating point Exception",
    "Error: Virtualization Exception",
    "Error: Control Protection Exception",
    "Interrupt: Reserved (0x16)",
    "Interrupt: Reserved (0x17)",
    "Interrupt: Reserved (0x18)",
    "Interrupt: Reserved (0x19)",
    "Interrupt: Reserved (0x1A)",
    "Interrupt: Reserved (0x1B)",
    "Error: Hypervisor Injection Exception",
    "Error: VMM Communication Exception",
    "Error: Security Exception",
    "Interrupt: Reserved (0x31)"
};

void __cdecl x86_ISR_Handler(ISR_Register_t *registers)
{
    if (g_ISRHandlers[registers->interrupt_number] != NULL)
    {
        g_ISRHandlers[registers->interrupt_number](registers);
    }
    else if (registers->interrupt_number >= 32)
    {
        scr_colorMode = COLOR(BLACK, YELLOW);
        printf("Unhandled kernel interrupt %d\r\n", registers->interrupt_number);
        printf("eax=%x ebx=%x ecx=%x edx=%x esi=%x edi=%x\r\n", registers->eax, registers->ebx, registers->ecx, registers->edx, registers->esi, registers->edi);
        printf("interrupt=%d errorcode=%x\r\n", registers->interrupt_number, registers->error);
    }
    else // critical system set interrupts, do kill execution in case of interrupt with no resolution
    {
        scr_colorMode = COLOR(BLACK, RED);
        printf("Unhandled Exception %d %s\r\n", registers->interrupt_number, ISR_Exception_Messages[registers->interrupt_number]);

        printf("eax=%x ebx=%x ecx=%x edx=%x esi=%x edi=%x\r\n", registers->eax, registers->ebx, registers->ecx, registers->edx, registers->esi, registers->edi);
        printf("interrupt=%d errorcode=%x\r\n", registers->interrupt_number, registers->error);
        
        x86_Death_After_Interrupt();
    }
}

void ISR_RegisterHandler(int interrupt_number, ISR_HandlerF_t handler)
{
    g_ISRHandlers[interrupt_number] = handler;
    IDT_EnableGate(interrupt_number, true);
}
