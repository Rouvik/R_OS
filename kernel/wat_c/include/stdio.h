/*  Provides C like Standard IO functions using the underlying TTY driver
    Copyright (C) 2024  Rouvik Maji

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

    Contact: majirouvik@gmail.com
*/

#ifndef __STDIO__
#define __STDIO__

#include "stdint.h"
#include "../drivers/tty.h"
#include "include/x86_inc.h"

int printInt(int x)
{
    if (!x)             // if the number is zero
    {
        putc('0');
        return 1;
    }
    
    char numStr[10] = {0};
    int i = 8;
    while (x > 0)
    {
        numStr[i--] = '0' + (x % 10);
        x /= 10;
    }

    puts(numStr + i + 1);
    
    return 10 - i - 2;
}

int printLongInt(uint64_t x)
{
    if (!x)                 // if the number is zero
    {
        putc('0');
        return 1;
    }
    
    char numStr[20] = {0};
    int i = 18;
    while (x > 0)
    {
        uint32_t rem;
        x86_div_u64(x, 10, &x, &rem);
        numStr[i--] = '0' + rem;
    }

    puts(numStr + i + 1);
    
    return 0;

    return 20 - i - 2;
}

int printHexInt(uint32_t ptr)
{
    if (!ptr)
    {
        puts("0x00");
        return 4;
    }
    
    char hexStr[8] = {0};
    int i = 6;

    while (ptr > 0)
    {
        int x = ptr % 16;
        if (x < 10)
        {
            hexStr[i--] = '0' + x;
        }
        else
        {
            hexStr[i--] = 'a' + (x - 10);
        }

        ptr /= 16;
    }

    puts("0x");
    puts(hexStr + i + 1);
    
    return 8 - i;               // no -2 since we are also printing the preceding "0x"
}

int _cdecl printf(const char *fmt, ...)
{
    uint32_t *argp = (uint32_t *)&fmt;
    argp += sizeof(fmt) / sizeof(int);          // now argp points to 1st var argument

    uint32_t charsPrinted = 0;

    for (char *i = (char *)fmt; *i != 0; i++)
    {
        if (*i == '%')
        {
            switch (*(i + 1))
            {
            case 's':
                charsPrinted += puts((const char *)(*argp));
                argp++;
                break;

            case 'd':
                charsPrinted += printInt(*argp);
                argp++;
                break;

            case 'l':
                i++;                    // also consume the suceeding d as per the printing format
                if (*(i + 1) != 'd')
                {
                    break;
                }
                
                charsPrinted += printLongInt((uint64_t)(*argp));
                argp += 2;              // since it's a long int and will consume 2 32 bit spaces
                break;

            case 'c':
                putc((char)*argp);
                charsPrinted++;
                argp++;
                break;

            case 'p':
                charsPrinted += printHexInt(*argp);
                argp++;
                break;

            case 'x':
                charsPrinted += printHexInt(*argp);
                argp++;
                break;
            
            case '%':
                charsPrinted++;
                putc('%');
                break;
            }

            i++;
        }
        else
        {
            putc(*i);
            charsPrinted++;
        }
    }

    return charsPrinted;
}

#endif // __STDIO__
