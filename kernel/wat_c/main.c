#include "include/stdint.h"
#include "drivers/tty.h"
#include "include/stdio.h"
#include "include/x86_inc.h"

void readMemory()
{
    uint8_t *elemsPtr = 0x7E09;
    uint64_t *addp = 0x8200;

    for (int i = 0; i < *elemsPtr; i++)
    {
        uint64_t baseAddr = *addp;
        addp++;
        uint64_t lengthPtr = *addp;
        addp++;
        uint64_t typeAndExtAttr = *addp;
        addp++;

        uint32_t type = 0x00000000FFFFFFFF & typeAndExtAttr;
        uint32_t extAttr = 0xFFFFFFFF00000000 & typeAndExtAttr;

        printf("Base Address: %ld Length: %ld Type: %d Extended Attribute: %d\r\n", baseAddr, lengthPtr, type, extAttr);
    }
}

int _cdecl kmain()
{
    scr_colorMode = COLOR(BLACK, BLACK);
    clearTTY();
    scr_colorMode = COLOR(BLACK, WHITE);
    puts("Welcome to RKernel Version 1.2 (alpha)\n\r"
                "This message is written from RKernel TTY driver\n\r"
                "If you see any errors...please restart the OS or contact developer\n\r\n"
                "The support command line utility and rest of the drivers are work in progress...\n\r\n");

    scr_colorMode = COLOR(BLACK, LIGHT_GREEN);
    puts("Created/Last Updated by Rouvik Maji 4/09/2024 -- 2:05PM IST\n\r");

    // scr_colorMode = COLOR(BLACK, RED);
    // puts("Note to self: The memory table is stored at 0x8200 and its size(number of 24 bit elements) at 0x7e09\n\r");

    scr_colorMode = COLOR(BLACK, CYAN);

    puts("\nMemory map:\r\n");

    readMemory();       // print the system memory map

    while (true);
}
