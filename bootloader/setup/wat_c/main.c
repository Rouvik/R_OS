#include "include/stdint.h"
#include "include/stdio.h"

int _cdecl cstart_(uint16_t bootDrive)
{
    puts("Hello world from C!\r\n");

    while (true);
}
