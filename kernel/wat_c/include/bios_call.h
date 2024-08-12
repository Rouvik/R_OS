#ifndef __BIOS_CALL__
#define __BIOS_CALL__

#include "stdint.h"

// TTY
void _cdecl bios_writeCharTTY(char ch, uint8_t page);

#endif // __BIOS_CALL__
