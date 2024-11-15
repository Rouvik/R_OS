#ifndef __IRQ__
#define __IRQ__

#include "../drivers/8259_pic.h"
#include "isr.h"

#define PIC_REMAP_OFFSET 0x20   // offset at 32, as first 32 are system error interrupts

typedef void (*IRQ_HandlerF_t)(ISR_Register_t *regs);

void IRQ_Initialise();
void IRQ_RegisterHandler(int irq, IRQ_HandlerF_t handler);
void IRQ_Handler(ISR_Register_t *reg);

#endif // __IRQ__
