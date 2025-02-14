#include "include/stdint.h"
#include "drivers/tty.h"
#include "stdlib/stdio.h"
#include "include/x86_inc.h"
#include "./interrupts/idt.h"
#include "./interrupts/isr.h"
#include "./interrupts/irq.h"
#include "drivers/vga.h"

typedef struct memEntry
{
    uint64_t baseAddr;
    uint64_t length;
    uint32_t type;
    uint32_t extAttr;
} memEntry_t;

void memcpy(uint8_t *dest, uint8_t *src, int count)
{
    for (int i = 0; i < count; i++)
    {
        dest[i] = src[i];
    }
}

void readVBEInfo()
{
    VbeInfoBlock_t *block = (VbeInfoBlock_t *)(*((uint16_t *)0x7E0C));

    char Sig[5];
    memcpy(Sig, block->VbeSignature, 4);
    Sig[4] = 0;

    printf("Signature: %s\r\n"
           "Version: %x\r\n"
           "OemNamePtr Seg: %p\r\n"
           "OemNamePtr Base: %p\r\n"
           "Capabilities:\r\n",
           Sig, block->VbeVersion, block->OemStringPtr[1], block->OemStringPtr[0]);

    // printf("%d %d %d %d", block->Capabilities[3], block->Capabilities[2], block->Capabilities[1], block->Capabilities[0]);

    printf("%d", (uint32_t)block->Capabilities);

    printf("\r\nVideoModePtr Seg: %p\r\n"
           "VideoModePtr Off: %p\r\n"
           "TotalMemory: %d\r\n",
           block->VideoModePtr[1], block->VideoModePtr[0], block->CountOf64KBlocks);

    // scr_colorMode = COLOR(BLACK, GREEN);

    // uint16_t *modePtr = (block->VideoModePtr[1] << 4) + block->VideoModePtr[0];
    // for (int i = 0; modePtr[i] != 0xFFFF; i++)
    // {
    //     printf("Mode: %d\t", modePtr[i]);
    // }
}

void readVBEModeInfo()
{
    VbeModeInfoBlock_t *block = (VbeModeInfoBlock_t *)(*((uint16_t *)0x7E0E));

    scr_colorMode = COLOR(BLACK, GREEN);

    printf( //"Mode Attributes: %x\r\n"
            //    "Window A: %x\r\n"
            //    "Window B: %x\r\n"
            //    "Granularity: %x\r\n"
            //    "Window Size: %x\r\n"
            //    "Segment A: %x\r\n"
            //    "Segment B: %x\r\n"
            //    "Win Func Ptr: %x\r\n"
        "Pitch: %d\r\n"
        "Width: %d\r\n"
        "Height: %d\r\n"
        "W Char: %d\r\n"
        "Y Char: %d\r\n"
        //    "Planes: %d\r\n"
        "BPP: %d\r\n"
        "Banks: %x\r\n"
        "Memory Model: %d\r\n"
        "Bank Size: %x\r\n"
        "Image Pages: %x\r\n"
        "Reserved0: %x\r\n"
        "Red Mask: %d\r\n"
        "Red Position: %d\r\n"
        "Green Mask: %d\r\n"
        "Green Position: %d\r\n"
        "Blue Mask: %d\r\n"
        "Blue Position: %d\r\n"
        "Reserved Mask: %d\r\n"
        "Reserved Position: %d\r\n"
        "Direct Color Attributes: %x\r\n"
        "Framebuffer: %x\r\n"
        "Off Screen Mem Off: %x\r\n"
        "Off Screen Mem Size: %x\r\n",
        /*block->attributes, block->window_a, block->window_b, block->granularity, block->window_size, block->segment_a, block->segment_b, block->win_func_ptr,*/ block->pitch, block->width, block->height, block->w_char, block->y_char, /*block->planes,*/ block->bpp, block->banks, block->memory_model, block->bank_size, block->image_pages, block->reserved0, block->red_mask, block->red_position, block->green_mask, block->green_position, block->blue_mask, block->blue_position, block->reserved_mask, block->reserved_position, block->direct_color_attributes, block->framebuffer, block->off_screen_mem_off, block->off_screen_mem_size);

    // scr_colorMode = COLOR(BLACK, CYAN);

    // printf("Width: %d Height: %d BPP: %d Framebuffer: %p\r\n", block->width, block->height, block->bpp, block->framebuffer);
}

void readMemory()
{
    uint8_t *elemsPtr = (uint8_t *)0x7E09;
    memEntry_t *addp = (memEntry_t *)(*((uint16_t *)0x7E0A));

    for (int i = 0; i < *elemsPtr; i++)
    {
        printf("Base Address: %ld Length: %ld Type: %d Extended Attribute: %d\r\n", addp->baseAddr, addp->length, addp->type, addp->extAttr);
        addp++;
    }
}

// interrupts ------------------
void timer(ISR_Register_t *reg)
{
    (void)reg;
    // putc('.');
}

void kbd(ISR_Register_t *reg)
{
    (void)reg;
    uint8_t kbd_data = x86_inb(0x60); // read the kbd buffer to actually clear the interrupt
    printf("Kdb: %x ", kbd_data);
}

int __cdecl kmain(int PageDirectoryBuffer, int PageTableBuffer, int IdentityPageTableBuffer, int driveNumber)
{

    //
    // PAGING WORKS, BUT NOW TTY ADDRESS 0xB8000 IS NOT MAPPED TO A REAL ADDRESS, FIX THAT (IDENTITY MAP IT!)
    //


    scr_colorMode = COLOR(BLACK, BLACK);
    // clearTTY();
    /*
    scr_colorMode = COLOR(BLACK, WHITE);
    puts("Welcome to RKernel Version 1.2 (alpha)\n\r"
         "This message is written from RKernel TTY driver\n\r"
         "If you see any errors...please restart the OS or contact developer\n\r\n"
         "The support command line utility and rest of the drivers are work in progress...\n\r");

    scr_colorMode = COLOR(BLACK, LIGHT_GREEN);
    puts("Created/Last Updated by Rouvik Maji 12/11/2024 -- 9:27PM IST\n\r");

    // scr_colorMode = COLOR(BLACK, RED);
    // puts("Note to self: The memory table is stored at 0x8200 and its size(number of 24 bit elements) at 0x7e09\n\r");

    scr_colorMode = COLOR(BLACK, CYAN);

    puts("\nMemory map:\r\n");

    readMemory(); // print the system memory map

    putc('\n');

    ISR_Initialise();   // initialise the interrupt table
    IDT_LoadIDTTable(); // load the interrupt table to idtr
    IRQ_Initialise();   // initialise the Interrupt requests
    IRQ_RegisterHandler(0, timer);
    IRQ_RegisterHandler(1, kbd);

    scr_colorMode = COLOR(BLACK, YELLOW);

    clearTTY();
    scr_Y = 0;
    // readVBEInfo();
    // puts("\r\n");
    readVBEModeInfo();

    scr_colorMode = COLOR(BLACK, RED);
    printf("Selected color mode: %d\r\n", VGA_getSelectedMode());

    // while (1);

    VGA_getModePointer();
    VGA_getSelectedMode();

    if (!VGA_setupGraphicalMode())
    {
        while (true); // crash indefinetly
    }

    for (int y = 0; y < VGA_modeInfo->height; y++)
    {
        for (int x = 0; x < VGA_modeInfo->width; x++)
        {
            MVGA_putPixelRGB(x, y, x, y, x ^ y);
        }
    }

    for (int i = 0; i < 2000000; i++);              // a long enough wait loop

    for (int y = 0; y < VGA_modeInfo->height; y++)
    {
        for (int x = 0; x < VGA_modeInfo->width; x++)
        {
            int x0 = x - VGA_modeInfo->width / 2;
            int y0 = y - VGA_modeInfo->height / 2;
            if (x0 * x0 + y0 * y0 < 14400)
            {
                MVGA_putPixelRGB(x, y, x, y, x ^ y);
            }
            else
            {
                MVGA_putPixelRGB(x, y, 0, 0, 0);
            }
        }
    }
        */

    while (true)
        ; // halt
}
