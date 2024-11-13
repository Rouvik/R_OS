#ifndef __SYS_IO__
#define __SYS_IO__

#include "../include/x86_inc.h"

#define UNUSED_PORT 0x80

void x86_iowait();

#endif // __SYS_IO__