#include "isr.h"

void _cdecl x86_ISR_Handler(ISR_Registers_t *registers)
{
    scr_colorMode = COLOR(BLACK, RED);
    printf("Interrupt number: %d\r\n", registers->interrupt_number);
}
