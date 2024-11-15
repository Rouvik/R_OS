#include "irq.h"

IRQ_HandlerF_t g_IRQHandlers[16];

void IRQ_Handler(ISR_Register_t *reg)
{
    int irq = reg->interrupt_number - PIC_REMAP_OFFSET;

    if (g_IRQHandlers[irq] != NULL)
    {
        g_IRQHandlers[irq](reg);
    }
    else
    {
        scr_colorMode = COLOR(BLACK, YELLOW);
        printf("Unhandled Hardware interrupt(IRQ): %d\r\n", irq);
        printf("eax=%x ebx=%x ecx=%x edx=%x esi=%x edi=%x\r\n", reg->eax, reg->ebx, reg->ecx, reg->edx, reg->esi, reg->edi);
        printf("interrupt=%d errorcode=%x\r\n", reg->interrupt_number, reg->error);
    }

    // just remove the interrupt with highest priority on handling it
    PIC_send_non_specific_EOI(irq);
}

void IRQ_RegisterHandler(int irq, IRQ_HandlerF_t handler)
{
    g_IRQHandlers[irq] = handler;
}

void IRQ_Initialise()
{
    PIC_configure(PIC_REMAP_OFFSET, PIC_REMAP_OFFSET + 8);
    
    for (int i = 0; i < 16; i++)
    {
        ISR_RegisterHandler(PIC_REMAP_OFFSET + i, IRQ_Handler);
    }
    
    x86_sti();
}
