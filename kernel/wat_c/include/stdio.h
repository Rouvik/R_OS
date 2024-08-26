#ifndef __STDIO__
#define __STDIO__

#include "stdint.h"

uint8_t *video_Buffer = (uint8_t *)0xB8000;
uint32_t loc = 0;

void putc(char ch)
{
    video_Buffer[loc] = ch;
    loc += 2;
}

void puts(const char *msg)
{
    for (char *i = (char *)msg; *i != 0; i++)
    {
        video_Buffer[loc] = *i;
        loc += 2;
    }
}

#endif // __STDIO__
