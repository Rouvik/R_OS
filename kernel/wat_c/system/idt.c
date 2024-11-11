#include "idt.h"

#include "../include/stdint.h"
#include "../include/stdio.h"

IDTEntry_t g_IDT[256];
IDTDesciptor_t g_IDTDescriptor = { .limit = sizeof(g_IDT) - 1, .ptr = g_IDT };

void IDT_SetGate(int interrupt_num, void *base, uint16_t segmentDescriptor, uint8_t flags)
{
    g_IDT[interrupt_num].baseLow = ((uint32_t)base) & 0xFFFF;
    g_IDT[interrupt_num].segmentSelector = segmentDescriptor;
    g_IDT[interrupt_num].reserved = 0;
    g_IDT[interrupt_num].flags = IDT_FLAG_PRESENT | flags;
    g_IDT[interrupt_num].baseHigh = ((uint32_t)base >> 16) & 0xFFFF;
}

void IDT_LoadIDTTable()
{
    x86_IDT_Load(&g_IDTDescriptor);
}

void IDT_EnableGate(int interrupt_num, bool enable)
{
    if (enable)
    {
        g_IDT[interrupt_num].flags |= IDT_FLAG_PRESENT;
    }
    else
    {
        g_IDT[interrupt_num].flags &= ~IDT_FLAG_PRESENT;
    }
}
