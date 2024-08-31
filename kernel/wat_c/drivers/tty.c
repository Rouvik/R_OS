/*  TTY driver for RKernel, provides a simple rudimentary interface to write to 0xB8000 for TTY services
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

#include "tty.h"

// TTY Video elements
uint8_t *video_Buffer = (uint8_t *)0xB8000;
uint8_t scr_X = 0;
uint8_t scr_Y = 0;

#define TEMP_TTY_POS(x, y) \
    (2 * (y * SCREEN_WIDTH + x))

uint8_t scr_colorMode = 0x00; // default = white on black

void clearTTY()
{
    for (uint32_t i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT * 2; i += 2)
    {
        video_Buffer[i] = ' ';
        video_Buffer[i + 1] = scr_colorMode;
    }
}

void putc(char ch)
{
    switch (ch)
    {
    case '\n':
        scr_Y++;
        return;     // get out of the function straight

    case '\r':
        scr_X = 0;
        return;     // get out of the function straight
    
    case '\t':
        for (uint8_t i = 0; i < 4 - (scr_X % 4); i++)
        {
            video_Buffer[TEMP_TTY_POS(scr_X, scr_Y)] = ' ';
            video_Buffer[TEMP_TTY_POS(scr_X, scr_Y) + 1] = scr_colorMode;
            if (++scr_X > SCREEN_WIDTH)
            {
                scr_X = 0;
                scr_Y++;
            }
        }
        return;     // get out of the function straight

    default:
        video_Buffer[TEMP_TTY_POS(scr_X, scr_Y)] = ch;
        video_Buffer[TEMP_TTY_POS(scr_X, scr_Y) + 1] = scr_colorMode;

        if (++scr_X > SCREEN_WIDTH)
        {
            scr_X = 0;
            scr_Y++;
        }
    }
}

void mputc(uint8_t x, uint8_t y, char ch)
{
    scr_X = x % SCREEN_WIDTH;
    scr_Y = y % SCREEN_HEIGHT;
    putc(ch);
}

uint32_t puts(const char *msg)
{
    uint32_t charsPrinted = 0;
    for (char *i = (char *)msg; *i != 0; i++)
    {
        charsPrinted++;
        putc(*i);
    }

    return charsPrinted;
}

uint32_t mputs(uint8_t x, uint8_t y, const char *msg)
{
    scr_X = x % SCREEN_WIDTH;
    scr_Y = y % SCREEN_HEIGHT;

    return puts(msg);
}

void setCur(uint8_t x, uint8_t y, uint8_t color)
{
    scr_X = x % SCREEN_WIDTH;
    scr_Y = y % SCREEN_HEIGHT;
    scr_colorMode = color;
}

void setPos(uint8_t x, uint8_t y)
{
    scr_X = x % SCREEN_WIDTH;
    scr_Y = y % SCREEN_HEIGHT;
}

void setX(uint8_t x)
{
    scr_X = x % SCREEN_WIDTH;
}

void setY(uint8_t y)
{
    scr_Y = y % SCREEN_HEIGHT;
}

#undef TEMP_TTY_POS // make sure this does not exist later on
