#ifndef __8259_PIC__
#define __8259_PIC__

#include "../include/stdint.h"
#include "../include/x86_inc.h"
#include "../system/io.h"

#define PIC_MASTER_COMMAND 0x20
#define PIC_MASTER_DATA 0x21
#define PIC_SLAVE_COMMAND 0xA0
#define PIC_SLAVE_DATA 0xA1

extern enum
{
    PIC_ICW1_ICW4 = 0x01,
    PIC_ICW1_SINGLE = 0x02,
    PIC_ICW1_INTERVAL4 = 0x04,
    PIC_ICW1_LEVEL = 0x08,
    PIC_ICW1_INIT = 0x10
} PIC_ICW1;


extern enum
{
    PIC_ICW4_8086 = 0x01,
    PIC_ICW4_AUTO = 0x02,
    PIC_ICW4_BUF_SLAVE = 0x08,
    PIC_ICW4_BUF_MASTER = 0x0C,
    PIC_ICW4_SFNM = 0x10
} PIC_ICW4;

extern enum
{
    PIC_CMD_SPECIFIC_END_OF_INT = 0x60,
    PIC_CMD_NON_SPECIFIC_END_OF_INT = 0x20,
    PIC_CMD_READ_IRR = 0x0A,
    PIC_CMD_READ_ISR = 0x0B
} PIC_CMD;

void PIC_configure(uint32_t offset1, uint32_t offset2);

void PIC_mask(uint8_t irq, bool enable);
void PIC_disable();

void PIC_send_specific_EOI(uint8_t irq);
void PIC_send_non_specific_EOI(uint8_t irq);

uint16_t PIC_read_IRR();
uint16_t PIC_read_ISR();

#endif // __8259_PIC__
