#ifndef __IDT__
#define __IDT__

#include "../include/stdint.h"

#pragma pack(1) // alternative to __atribute((packed)) in open watcom

typedef struct IDTEntry
{
    uint16_t baseLow;
    uint16_t segmentSelector;
    uint8_t reserved;
    uint8_t flags;
    uint16_t baseHigh;
} IDTEntry_t;

typedef struct IDTDesciptor
{
    uint16_t limit;
    IDTEntry_t *ptr;
} IDTDesciptor_t;

#pragma pack() // reset packing

typedef enum {
    IDT_FLAG_TASK_GATE = 0x5,
    IDT_FLAG_16_INTERRUPT_GATE = 0x6,
    IDT_FLAG_16_TRAP_GATE = 0x7,
    IDT_FLAG_32_INTERRUPT_GATE = 0xE,
    IDT_FLAG_32_TRAP_GATE = 0xF,

    IDT_FLAG_DPL_RING0 = (0 << 5),
    IDT_FLAG_DPL_RING1 = (1 << 5),
    IDT_FLAG_DPL_RING2 = (2 << 5),

    IDT_FLAG_PRESENT = 0x80
} IDT_FLAGS;

void IDT_SetGate(int interrupt_num, void *base, uint16_t segmentDescriptor, uint8_t flags);

void IDT_EnableGate(int interrupt_num, bool enable);

void IDT_LoadIDTTable();

#endif // __IDT__
