#include "8259_pic.h"

void PIC_configure(uint32_t offset_master, uint32_t offset_slave)
{
    // cascade mode initialisation
    x86_outb(PIC_MASTER_COMMAND, PIC_ICW1_INIT | PIC_ICW1_ICW4);
    x86_iowait();
    x86_outb(PIC_SLAVE_COMMAND, PIC_ICW1_INIT | PIC_ICW1_ICW4);
    x86_iowait();

    // master PIC vector offset
    x86_outb(PIC_MASTER_DATA, offset_master);
    x86_iowait();

    // slave PIC vector offset
    x86_outb(PIC_SLAVE_DATA, offset_slave);
    x86_iowait();

    // tell master PIC that slave PIC is at 0000 0010
    x86_outb(PIC_MASTER_DATA, 4);
    x86_iowait();
    // tell slave PIC its cascade identity
    x86_outb(PIC_SLAVE_DATA, 2);
    x86_iowait();

    // notify master its 8086
    x86_outb(PIC_MASTER_DATA, PIC_ICW4_8086);
    x86_iowait();

    // notify slave its 8086
    x86_outb(PIC_SLAVE_DATA, PIC_ICW4_8086);
    x86_iowait();

    // disable masking for now
    x86_outb(PIC_MASTER_DATA, 0);
    x86_outb(PIC_SLAVE_DATA, 0);
}

void PIC_mask(uint8_t irq, bool enable)
{
    uint16_t port;
    if (irq < 8)
    {
        port = PIC_MASTER_DATA;
    }
    else
    {
        port = PIC_SLAVE_DATA;
        irq -= 8;
    }
    uint8_t mask = x86_inb(port);
    if (enable)
    {
        x86_outb(port, mask | (1 << irq));
    }
    else
    {
        x86_outb(port, mask & ~(1 << irq));
    }
}

void PIC_disable()
{
    x86_outb(PIC_MASTER_DATA, 0xff);
    x86_iowait();
    x86_outb(PIC_SLAVE_DATA, 0xff);
    x86_iowait();
}

void PIC_send_specific_EOI(uint8_t irq)
{
    if (irq >= 8)
    {
        x86_outb(PIC_SLAVE_COMMAND, PIC_CMD_SPECIFIC_END_OF_INT | (irq - 8));
        x86_outb(PIC_MASTER_COMMAND, PIC_CMD_SPECIFIC_END_OF_INT | 2);
    }
    else
    {
        x86_outb(PIC_MASTER_COMMAND, PIC_CMD_SPECIFIC_END_OF_INT | irq);
    }
}

void PIC_send_non_specific_EOI(uint8_t irq)
{
    if (irq >= 8)
    {
        x86_outb(PIC_SLAVE_COMMAND, PIC_CMD_NON_SPECIFIC_END_OF_INT);
    }
    x86_outb(PIC_MASTER_COMMAND, PIC_CMD_NON_SPECIFIC_END_OF_INT);
}

uint16_t PIC_read_IRR()
{
    x86_outb(PIC_MASTER_COMMAND, PIC_CMD_READ_IRR);
    x86_outb(PIC_SLAVE_COMMAND, PIC_CMD_READ_IRR);
    return x86_inb(PIC_MASTER_COMMAND) | x86_inb(PIC_SLAVE_COMMAND) << 8;
}

uint16_t PIC_read_ISR()
{
    x86_outb(PIC_MASTER_COMMAND, PIC_CMD_READ_ISR);
    x86_outb(PIC_SLAVE_COMMAND, PIC_CMD_READ_ISR);
    return x86_inb(PIC_MASTER_COMMAND) | x86_inb(PIC_SLAVE_COMMAND) << 8;
}
