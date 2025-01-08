#include "vga.h"

VbeModeInfoBlock_t *VGA_modeInfo;
uint16_t VGA_modeID;
uint32_t VGA_bpp_in_bytes;

VGA_colorMask_t VGA_colorMask;

uint16_t VGA_setVideoMode(uint16_t mode)
{
    uint16_t ret = x86_setVideoMode(mode); // call the underlying x86 10h interrupt in real mode
    IDT_LoadIDTTable();                    // load the interrupt table to idtr
    IRQ_Initialise();                      // initialise the Interrupt requests
    return ret;
}

VbeModeInfoBlock_t *VGA_getModePointer()
{
    VGA_modeInfo = (VbeModeInfoBlock_t *)(*((uint16_t *)0x7E0E));
    
    int div = VGA_modeInfo->bpp / 8;
    if (VGA_modeInfo->bpp % 8)
    {
        div++;
    }
    VGA_bpp_in_bytes = div;

    VGA_colorMask.r_mask = (1U << VGA_modeInfo->red_mask) - 1;
    VGA_colorMask.g_mask = (1U << VGA_modeInfo->green_mask) - 1;
    VGA_colorMask.b_mask = (1U << VGA_modeInfo->blue_mask) - 1;
    return VGA_modeInfo;
}

uint16_t VGA_getSelectedMode()
{
    VGA_modeID = *((uint16_t *)0x7E10);
    return VGA_modeID;
}

bool VGA_setupGraphicalMode()
{
    VGA_getSelectedMode();
    VGA_getModePointer();

    if (!VGA_setVideoMode(VGA_modeID))
    {
        scr_X = 0;
        scr_Y = 0;
        scr_colorMode = COLOR(BLACK, RED);
        puts("[FATAL ERROR] Failed to set graphical mode\r\n");
        return false;
    }

    return true;
}

void VGA_putPixel(uint32_t x, uint32_t y, VGA_vec3_t color)
{
    uint8_t *address = (uint8_t *)VGA_modeInfo->framebuffer + (y * VGA_modeInfo->pitch) + (x * VGA_bpp_in_bytes);
    *((uint32_t *)address) = ((color.r & VGA_colorMask.r_mask) << VGA_modeInfo->red_position) | ((color.g & VGA_colorMask.g_mask) << VGA_modeInfo->green_position) | ((color.b & VGA_colorMask.b_mask) << VGA_modeInfo->blue_position);
}

void VGA_putPixelNoMask(uint32_t x, uint32_t y, VGA_vec3_t color)
{
    uint8_t *address = (uint8_t *)VGA_modeInfo->framebuffer + (y * VGA_modeInfo->pitch) + (x * VGA_bpp_in_bytes);
    *((uint32_t *)address) = (color.r << VGA_modeInfo->red_position) | (color.g << VGA_modeInfo->green_position) | (color.b << VGA_modeInfo->blue_position);
}

