#ifndef __VGA__
#define __VGA__

#include "../include/x86_inc.h"
#include "../interrupts/idt.h"
#include "../interrupts/irq.h"

/**
 * @brief Sets the VGA mode by actually jumping into 16-bit REAL mode BIOS calls
 * @warning This function is delicate and returns to the 16-bit real mode execution thus
 * requiring a lot of extra cleanup after coming back abd should be dealt with carefully and used as less often as possible
 * 
 * @param mode The mode number to set the VGA to
 * @return uint16_t success = 1 and failure = 0
 */
uint16_t VGA_setVideoMode(uint16_t mode);

#endif // __VGA__
