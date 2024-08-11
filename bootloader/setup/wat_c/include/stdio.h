#ifndef __STDIO__
#define __STDIO__

#include "bios_call.h"

/**
 * @brief Prints a character to BIOS TTY
 * @note Try to call bios_writeCharTTY instead with page=0
 *       that's what it actually does
 * 
 * @param ch The character to print
 */
void putc(char ch)
{
    bios_writeCharTTY(ch, 0);
}

/**
 * @brief Prints a string to BIOS TTY
 * 
 * @param str The string to print
 */
void puts(const char *str)
{
    while (*str)
    {
        bios_writeCharTTY(*str, 0);
        ++str;
    }
}

#endif // __STDIO__
