#include "idt.h"

IDTEntry_t g_IDT[256];

IDTDesciptor_t g_IDTDescriptor = { sizeof(g_IDT) - 1, g_IDT };

void IDT_SetGate(int interrupt, void *base, uint16_t segmentDescriptor, uint8_t flags)
{
    g_IDT[interrupt].baseLow = ((uint32_t)base) & 0xFFFF;
    g_IDT[interrupt].segmentSelector = segmentDescriptor;
    g_IDT[interrupt].reserved = 0;
    g_IDT[interrupt].flags = flags;
    g_IDT[interrupt].baseHigh = ((uint32_t)base >> 16) & 0xFFFF;
}

void IDT_EnableGate(int interrupt, bool enable)
{
    if (enable)
    {
        g_IDT[interrupt].flags |= IDT_FLAG_PRESENT;
    }
    else
    {
        g_IDT[interrupt].flags &= ~IDT_FLAG_PRESENT;
    }
}