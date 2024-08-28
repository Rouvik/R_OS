#ifndef __DRIVER_TTY__
#define __DRIVER_TTY__

#include "../include/stdint.h"

// SCREEN CONSTRAINTS
const uint32_t SCREEN_WIDTH = 80;
const uint32_t SCREEN_HEIGHT = 25;

// TTY Video elements
extern uint8_t scr_X;
extern uint8_t scr_Y;

// BIOS color map
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHT_GRAY 7
#define DARK_GRAY 8
#define LIGHT_BLUE 9
#define LIGHT_GREEN A
#define LIGHT_CYAN B
#define LIGHT_RED C
#define LIGHT_MAGENTA D
#define YELLOW E
#define WHITE F

// Color definition macros
#define STDIO_CONCAT(arg1, arg2) arg1##arg2
#define STDIO_HEX(arg) STDIO_CONCAT(0x, arg)

#define COLOR(background, foreground) \
    STDIO_HEX(STDIO_CONCAT(background, foreground))

extern uint8_t scr_colorMode; // default = white on black

/**
 * @brief Clears the entire TTY output with the specified scr_colorMode
 */
void clearTTY();

/**
 * @brief Prints a character to the screen at scr_X and scr_Y
 *
 * @param ch The character to print
 */
void putc(char ch);

/**
 * @brief Moves cursor to position x, y and prints character ch
 *
 * @param x The x position on screen
 * @param y The y position on screen
 * @param ch The character to print
 */
void mputc(uint8_t x, uint8_t y, char ch);

/**
 * @brief Prints a string to TTY at scr_X and scr_Y
 *
 * @param msg The string to print
 */
void puts(const char *msg);

/**
 * @brief Moves the cursor to x, y and prints the string
 *
 * @param x The x position on screen
 * @param y The y position on screen
 * @param msg The string to print to screen
 */
void mputs(uint8_t x, uint8_t y, const char *msg);

/**
 * @brief Sets position and color details of the cursor
 *
 * @note Setting x and y here makes sure they exist within range SCREEN_WIDTH and SCREEN_HEIGHT
 *
 * @param x The x position on screen
 * @param y The y position on screen
 * @param color The cursor color from BIOS color table, preferable updated using COLOR(background, foreground)
 */
void setCur(uint8_t x, uint8_t y, uint8_t color);

/**
 * @brief Sets the cursor position
 *
 * @note Setting x and y here makes sure they exist within range SCREEN_WIDTH and SCREEN_HEIGHT
 *
 * @param x The x position on screen
 * @param y The y position on screen
 */
void setPos(uint8_t x, uint8_t y);

/**
 * @brief Sets the x position of cursor
 *
 * @note Setting x here makes sure it exist within SCREEN_WIDTH
 *
 * @param x The x position on screen
 */
void setX(uint8_t x);

/**
 * @brief Sets the y position of cursor
 *
 * @note Setting y here makes sure it exist within SCREEN_HEIGHT
 *
 * @param y The y position on screen
 */
void setY(uint8_t y);

#undef TEMP_TTY_POS // make sure this does not exist later on

#endif // __DRIVER_TTY__
