#include "include/stdint.h"
#include "drivers/tty.h"
#include "include/stdio.h"

int _cdecl kmain()
{
    scr_colorMode = COLOR(BLACK, BLACK);
    clearTTY();
    scr_colorMode = COLOR(BLACK, WHITE);
    puts("Welcome to RKernel Version 1.1 (alpha)\n\r"
                "This message is written from RKernel TTY driver\n\r"
                "If you see any errors...please restart the OS or contact developer\n\r\n"
                "The support command line utility and rest of the drivers are work in progress...\n\r\n");

    scr_colorMode = COLOR(BLACK, LIGHT_GREEN);
    puts("Created/Last Updated by Rouvik Maji 31/08/2024 -- 7:32PM IST\n\r");

    scr_colorMode = COLOR(BLACK, RED);
    puts("Note to self: The memory table is stored at 0x8200 and its size(number of 24 bit elements) at 0x7e09\n\r");

    scr_colorMode = COLOR(BLACK, CYAN);

    printf("\nTests for new stdio.h library\n\r"
                "Strings: %s\n\r"
                "Characters: %c\n\r"
                "Integers: %d\n\r"
                "Pointers: %p\n\r"
                "Escaped %% sign: %%\n\r",

                "Hello world!", 'A', 69, 0x7E00);

    while (true);
}
