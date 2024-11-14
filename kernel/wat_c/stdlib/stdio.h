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

#include "../include/stdint.h"
#include "../drivers/tty.h"
#include "../include/x86_inc.h"

/**
 * @brief Prints a 32 bit number to stdout
 * 
 * @param x The number to print to stdout
 * @return int The number of characters printed
 */
extern int printInt(int x);

/**
 * @brief Prints a 64 bit number to stdout
 * 
 * @param x The number to print to stdout
 * @return int The number of characters printed
 */
extern int printLongInt(uint64_t x);

/**
 * @brief Prints a 32bit number in hex to stdout
 * 
 * @param ptr The number to print to stdout, this function is mainly suited for printing pointers thus the naming
 * @return int The number of characters printed to stdout
 */
extern int printHexInt(uint32_t ptr);

/**
 * @brief Prints in C style formatting to stdout, presently TTY driver stdout
 * 
 * @param fmt The format string to refer to while printing @n
 *            Available formats:
 *              - %s = String
 *              - %d = 32 bit or smaller unsigned integers
 *              - %ld = 64 bit or smaller unsigned integers
 *              - %c = Charaters
 *              - %p = Pointers (hex print)
 *              - %x = Hexadecimal numbers
 *              - %% = escape sequence for '%' character
 * 
 * @param ... variadic input arguments
 * @return int The number of characters printed to stdout
 */
extern int __cdecl printf(const char *fmt, ...);

#endif // __STDIO__
