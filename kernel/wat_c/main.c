#include "include/stdint.h"
#include "drivers/tty.h"

int _cdecl kmain()
{
    scr_colorMode = COLOR(BLACK, BLACK);
    clearTTY();
    scr_colorMode = COLOR(BLACK, WHITE);
    puts("Welcome to RKernel Version 1.0 (alpha)\n\r"
                "This message is written from RKernel TTY driver\n\r"
                "If you see any errors...please restart the OS or contact developer\n\r\n"
                "The support command line utility and rest of the drivers are work in progress...\n\r\n");

    scr_colorMode = COLOR(BLACK, LIGHT_GREEN);
    puts("Created/Last Updated by Rouvik Maji 28/08/2024 -- 6:05PM IST\n\r");

    while (true);
}
