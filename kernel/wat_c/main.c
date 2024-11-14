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

void readMemory()
{
    uint8_t *elemsPtr = (uint8_t *)0x7E09;
    memEntry_t *addp = (memEntry_t *)0x8200;

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
    x86_inb(0x60); // read the kbd buffer to actually clear the interrupt
}

int __cdecl kmain()
{
    scr_colorMode = COLOR(BLACK, BLACK);
    clearTTY();
    scr_colorMode = COLOR(BLACK, WHITE);
    puts("Welcome to RKernel Version 1.2 (alpha)\n\r"
                "This message is written from RKernel TTY driver\n\r"
                "If you see any errors...please restart the OS or contact developer\n\r\n"
                "The support command line utility and rest of the drivers are work in progress...\n\r\n");

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

    while (true);       // halt
}
