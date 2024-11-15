#include "io.h"

void x86_iowait()
{
    x86_outb(UNUSED_PORT, 0);
}
