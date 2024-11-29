#include "include/stdint.h"
#include "drivers/tty.h"
#include "include/stdio.h"
#include "include/x86_inc.h"
#include "./interrupts/idt.h"
#include "./interrupts/isr.h"
#include "./interrupts/irq.h"

typedef struct memEntry
{
    uint64_t baseAddr;
    uint64_t length;
    uint32_t type;
    uint32_t extAttr;
} memEntry_t;

typedef struct VbeInfoBlock
{
    char VbeSignature[4];
    uint16_t VbeVersion;
    uint16_t OemStringPtr[2];
    uint8_t Capabilities[4];
    uint16_t VideoModePtr[2];

    uint16_t CountOf64KBlocks;
    uint16_t OEMSoftwareRevision;
    uint32_t OEMVendorNamePtr;
    uint32_t OEMProductNamePtr;
    uint32_t OEMProductRevisionPtr;
    uint8_t Reserved[222];
    uint8_t OEMData[256];
} _Packed VbeInfoBlock_t;

void memcpy(uint8_t *dest, uint8_t *src, int count)
{
    for (int i = 0; i < count; i++)
    {
        dest[i] = src[i];
    }
}

void readVBEInfo()
{
    VbeInfoBlock_t *block = (VbeInfoBlock_t *)(*((uint16_t *)0x7E0C));

    printf("VBE Loc: %p\r\n", block);

    char Sig[5];
    memcpy(Sig, block->VbeSignature, 4);
    Sig[4] = 0;

    printf("Signature: %s\r\n"
           "Version: %x\r\n"
           "OemNamePtr: %p\r\n"
           "Capabilities:\r\n",
           Sig, block->VbeVersion, block->OemStringPtr);

    // printf("%d %d %d %d", block->Capabilities[3], block->Capabilities[2], block->Capabilities[1], block->Capabilities[0]);
    
    printf("%d", (uint32_t)block->Capabilities);

    printf("\r\nVideoModePtr: %p\r\n"
           "TotalMemory: %d\r\n", block->VideoModePtr, block->CountOf64KBlocks);
}

void readMemory()
{
    uint8_t *elemsPtr = (uint8_t *)0x7E09;
    memEntry_t *addp = (memEntry_t *)(*((uint16_t *)0x7E0A));
    
    printf("Memtable Address: %p\r\n", addp);

    for (int i = 0; i < *elemsPtr; i++)
    {
        printf("Base Address: %ld Length: %ld Type: %d Extended Attribute: %d\r\n", addp->baseAddr, addp->length, addp->type, addp->extAttr);
        addp++;
    }
}

void timer(ISR_Register_t *reg)
{
    (void)reg;
    // putc('.');
}

void kbd(ISR_Register_t *reg)
{
    (void)reg;
    uint8_t kbd_data = x86_inb(0x60); // read the kbd buffer to actually clear the interrupt
    printf("Kdb: %x ", kbd_data);
}

int __cdecl kmain()
{
    scr_colorMode = COLOR(BLACK, BLACK);
    clearTTY();
    scr_colorMode = COLOR(BLACK, WHITE);
    puts("Welcome to RKernel Version 1.2 (alpha)\n\r"
                "This message is written from RKernel TTY driver\n\r"
                "If you see any errors...please restart the OS or contact developer\n\r\n"
                "The support command line utility and rest of the drivers are work in progress...\n\r");

    scr_colorMode = COLOR(BLACK, LIGHT_GREEN);
    puts("Created/Last Updated by Rouvik Maji 12/11/2024 -- 9:27PM IST\n\r");

    // scr_colorMode = COLOR(BLACK, RED);
    // puts("Note to self: The memory table is stored at 0x8200 and its size(number of 24 bit elements) at 0x7e09\n\r");

    scr_colorMode = COLOR(BLACK, CYAN);
    
    puts("\nMemory map:\r\n");

    readMemory();       // print the system memory map

    putc('\n');

    ISR_Initialise();   // initialise the interrupt table
    IDT_LoadIDTTable(); // load the interrupt table to idtr
    IRQ_Initialise();    // initialise the Interrupt requests
    IRQ_RegisterHandler(0, timer);
    IRQ_RegisterHandler(1, kbd);

    scr_colorMode = COLOR(BLACK, YELLOW);

    readVBEInfo();

    while (true);       // halt
}
