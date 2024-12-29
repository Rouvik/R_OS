#include "vga.h"

uint16_t VGA_setVideoMode(uint16_t mode)
{
    uint16_t ret = x86_setVideoMode(mode);                                                 // call the underlying x86 10h interrupt in real mode
    IDT_LoadIDTTable();                                                     // load the interrupt table to idtr
    IRQ_Initialise();                                                       // initialise the Interrupt requests
    return ret;
}
