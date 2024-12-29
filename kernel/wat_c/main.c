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

typedef struct VbeInfoBlock
{
    char VbeSignature[4];
    uint16_t VbeVersion;
    uint16_t OemStringPtr[2]; // segmented pointer
    uint8_t Capabilities[4];
    uint16_t VideoModePtr[2]; // segmented pointer

    uint16_t CountOf64KBlocks;
    uint16_t OEMSoftwareRevision;
    uint16_t OEMVendorNamePtr[2];      // segmented pointer
    uint16_t OEMProductNamePtr[2];     // segmented pointer
    uint16_t OEMProductRevisionPtr[2]; // segmented pointer
    uint8_t Reserved[222];
    uint8_t OEMData[256];
} _Packed VbeInfoBlock_t;

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

typedef struct VbeModeInfoBlock
{
    uint16_t attributes;  // deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
    uint8_t window_a;     // deprecated
    uint8_t window_b;     // deprecated
    uint16_t granularity; // deprecated; used while calculating bank numbers
    uint16_t window_size;
    uint16_t segment_a;
    uint16_t segment_b;
    uint32_t win_func_ptr; // deprecated; used to switch banks from protected mode without returning to real mode
    uint16_t pitch;        // number of bytes per horizontal line
    uint16_t width;        // width in pixels
    uint16_t height;       // height in pixels
    uint8_t w_char;        // unused...
    uint8_t y_char;        // ...
    uint8_t planes;
    uint8_t bpp;   // bits per pixel in this mode
    uint8_t banks; // deprecated; total number of banks in this mode
    uint8_t memory_model;
    uint8_t bank_size; // deprecated; size of a bank, almost always 64 KB but may be 16 KB...
    uint8_t image_pages;
    uint8_t reserved0;

    uint8_t red_mask;
    uint8_t red_position;
    uint8_t green_mask;
    uint8_t green_position;
    uint8_t blue_mask;
    uint8_t blue_position;
    uint8_t reserved_mask;
    uint8_t reserved_position;
    uint8_t direct_color_attributes;

    uint32_t framebuffer; // physical address of the linear frame buffer; write here to draw to the screen
    uint32_t off_screen_mem_off;
    uint16_t off_screen_mem_size; // size of memory in the framebuffer but not being displayed on the screen
    uint8_t reserved1[206];
} _Packed VbeModeInfoBlock_t;

void readVBEModeInfo()
{
    VbeModeInfoBlock_t *block = (VbeModeInfoBlock_t *)(*((uint16_t *)0x7E0E));

    // scr_colorMode = COLOR(WHITE, BLACK);

    // printf("Mode Attributes: %x\r\n"
    //        "Window A: %x\r\n"
    //        "Window B: %x\r\n"
    //        "Granularity: %x\r\n"
    //        "Window Size: %x\r\n"
    //        "Segment A: %x\r\n"
    //        "Segment B: %x\r\n"
    //        "Win Func Ptr: %x\r\n"
    //        "Pitch: %x\r\n"
    //        "Width: %d\r\n"
    //        "Height: %d\r\n"
    //        "W Char: %x\r\n"
    //        "Y Char: %x\r\n"
    //        "Planes: %d\r\n"
    //        "BPP: %d\r\n"
    //        "Banks: %x\r\n"
    //        "Memory Model: %d\r\n"
    //        "Bank Size: %x\r\n"
    //        "Image Pages: %x\r\n"
    //        "Reserved0: %x\r\n"
    //        "Red Mask: %x\r\n"
    //        "Red Position: %x\r\n"
    //        "Green Mask: %x\r\n"
    //        "Green Position: %x\r\n"
    //        "Blue Mask: %x\r\n"
    //        "Blue Position: %x\r\n"
    //        "Reserved Mask: %x\r\n"
    //        "Reserved Position: %x\r\n"
    //        "Direct Color Attributes: %x\r\n"
    //        "Framebuffer: %x\r\n"
    //        "Off Screen Mem Off: %x\r\n"
    //        "Off Screen Mem Size: %x\r\n",
    //        block->attributes, block->window_a, block->window_b, block->granularity, block->window_size, block->segment_a, block->segment_b, block->win_func_ptr, block->pitch, block->width, block->height, block->w_char, block->y_char, block->planes, block->bpp, block->banks, block->memory_model, block->bank_size, block->image_pages, block->reserved0, block->red_mask, block->red_position, block->green_mask, block->green_position, block->blue_mask, block->blue_position, block->reserved_mask, block->reserved_position, block->direct_color_attributes, block->framebuffer, block->off_screen_mem_off, block->off_screen_mem_size);

    scr_colorMode = COLOR(BLACK, CYAN);

    printf("Width: %d Height: %d BPP: %d Framebuffer: %p\r\n", block->width, block->height, block->bpp, block->framebuffer);
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

int __cdecl kmain()
{
    scr_colorMode = COLOR(BLACK, BLACK);
    clearTTY();
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
    readVBEInfo();
    puts("\r\n");
    readVBEModeInfo();

    scr_colorMode = COLOR(BLACK, RED);
    printf("Selected color mode: %d\r\n", *((uint16_t *)0x7E10));

    // this is the mode that gets selected by the entry VESA VGA driver shabby but good enough
    if (VGA_setVideoMode(19))
    {
        VbeModeInfoBlock_t *block = (VbeModeInfoBlock_t *)(*((uint16_t *)0x7E0E));

        for (int i = 0; i < 320 * 200; i++)
        {
            ((uint8_t *)block->framebuffer)[i] = 0x0a;
        }
    }
    else
    {
        scr_X = 0;
        scr_Y = 0;
        scr_colorMode = COLOR(BLACK, RED);
        puts("Failed to set video mode!\r\n");
    }

    for (int i = 0; i < 2000000; i++);             // a long enough wait loop

    // back to TTY mode from VGA mode!
    if (VGA_setVideoMode(3))
    {
        scr_X = 0;
        scr_Y = 0;
        scr_colorMode = COLOR(BLACK, GREEN);
        puts("Back to TTY mode!\r\n");
    }

    for (int i = 0; i < 6000000; i++);             // an even longer wait loop

    // test with RGB 5:5:5 mode!
    #define RGB_COLOR(red, green, blue) \
        (((red) << 10) | ((green) << 5) | (blue))

    // 640 x 400 rgb 5:5:5 color mode
    if (VGA_setVideoMode(272))
    {
        VbeModeInfoBlock_t *block = (VbeModeInfoBlock_t *)(*((uint16_t *)0x7E0E));

        for (int y = 0; y < 400; y++)
        {
            for (int x = 0; x < 640; x++)
            {
                ((uint16_t *)block->framebuffer)[640 * y + x] = RGB_COLOR(x & 0x1F, y & 0x1F, (x ^ y) & 0x1F);
            }
        }
    }

    for (int i = 0; i < 6000000; i++);             // an even longer wait loop

#define RGB_COLOR_321(red, green, blue) \
    (((red) << 16) | ((green) << 8) | (blue))

    VbeModeInfoBlock_t *block = NULL;

    // 640 x 400 rgb 32 bit x:8:8:8 color mode
    if (VGA_setVideoMode(321))
    {
        block = (VbeModeInfoBlock_t *)(*((uint16_t *)0x7E0E));
        for (int y = 0; y < 400; y++)
        {
            for (int x = 0; x < 640; x++)
            {
                ((uint32_t *)block->framebuffer)[640 * y + x] = RGB_COLOR_321(x, y, x ^ y);
            }
        }
    }
    else
    {
        while (1); // crash indefinetly
    }
    
    for (int i = 0; i < 6000000; i++);             // an even longer wait loop

    // 640 x 400 rgb 32 bit x:8:8:8 color mode
    // rendering a circle using simple distance formula
    for (int y = 0; y < 400; y++)
    {
        for (int x = 0; x < 640; x++)
        {
            int x0 = x - 320;
            int y0 = y - 200;
            ((uint32_t *)block->framebuffer)[640 * y + x] = x0 * x0 + y0 * y0 < 10000 ? RGB_COLOR_321(x, y, x ^ y) : 0;
        }
    }

    while (true)
        ; // halt
}
