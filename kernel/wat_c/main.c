#include "include/stdint.h"
#include "include/stdio.h"

int _cdecl kmain()
{
    // video_Buffer[0] = x;

    // char *msg = "                            Hello world from kernel!                            ";
    // for (char *i = msg; *i != 0; i++)
    // {
    //     video_Buffer[loc++] = *i;
    //     video_Buffer[loc++] = 12;
    // }

    puts("Hello world from kernel!");

    while (true);
}
