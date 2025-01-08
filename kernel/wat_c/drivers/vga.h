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
extern uint16_t VGA_setVideoMode(uint16_t mode);

/**
 * @brief Stores the VGA Video Mode information
 */
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

/**
 * @brief Stores the VGA Video Mode Graphical info mode (drawing and memory parameters)
 */
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

/**
 * @brief Returns the VGA mode pointer and sets the VGA_modeInfo variable with the mode info
 *
 * @return VbeModeInfoBlock_t* The mode info block pointer returned
 */
extern VbeModeInfoBlock_t *VGA_getModePointer();

/**
 * @brief Returns the VGA selected mode located and sets it to VGA_modeID
 *
 * @return uint16_t The mode ID returned
 */
extern uint16_t VGA_getSelectedMode();

/**
 * @brief A simple container structure to store 3D vector color information
 */
typedef struct VGA_vec3
{
    union
    {
        uint8_t x;
        uint8_t r;
    };

    union
    {
        uint8_t y;
        uint8_t g;
    };

    union
    {
        uint8_t z;
        uint8_t b;
    };
} VGA_vec3_t;

/**
 * @brief Storage struct for storing the generated color masks for internal rendering purposes
 */
typedef struct VGA_colorMasks
{
    uint32_t r_mask;
    uint32_t g_mask;
    uint32_t b_mask;
} VGA_colorMask_t;

/**
 * @brief Updated by the VGA_getModePointer function and stores the VbeModeInfoBlock_t * struct
 */
extern VbeModeInfoBlock_t *VGA_modeInfo;

/**
 * @brief Updated by the VGA_getSelectedMode function and stores the selected mode number for video
 */
extern uint16_t VGA_modeID;

/**
 * @brief Required internally for fast memory access, DO NOT TOUCH, contains the number of bytes of color data per pixel
 * @warning DO NOT CHANGE required by internal VGA driver to manipulate video memory
 */
extern uint32_t VGA_bpp_in_bytes;

/**
 * @brief Required internally for fast memory access, DO NOT TOUCH, contains the color mask for each channel
 * @warning DO NOT CHANGE required by internal VGA driver to manipulate video memory
 */
extern VGA_colorMask_t VGA_colorMask;

/**
 * @brief Calls the VGA_getSelectedMode and VGA_getModePointer functions to setup the video mode with VGA_setVideoMode
 * 
 * @warning This function calls a real mode function VGA_setVideoMode thus must be dealt with carefully
 * 
 * @return true Upon success
 * @return false Upon failure, error is printed to TTY if possible
 */
extern bool VGA_setupGraphicalMode();

/**
 * @brief Puts a pixel to the provided location in memory, function calls are slow and thus it is better to generally avoid it
 * 
 * @warning x and y are not checked for out of bounds
 * 
 * @param x The x position of the pixel
 * @param y The y position of the pixel
 * @param color The VGA_vec3 color structure of the color to store
 */
extern void VGA_putPixel(uint32_t x, uint32_t y, VGA_vec3_t color);

/**
 * @brief Puts a pixel to the provided location in memory without masking(making it a bit faster), function calls are slow and thus it is better to generally avoid it
 * 
 * @warning x and y are not checked for out of bounds
 * 
 * @param x The x position of the pixel
 * @param y The y position of the pixel
 * @param color The VGA_vec3 color structure of the color to store
 */
extern void VGA_putPixelNoMask(uint32_t x, uint32_t y, VGA_vec3_t color);

/**
 * @brief Macro version of VGA_putPixel with r, g, b values of color spread out, generally the fastest due to direct inline nature
 * 
 * @warning x and y are not checked for out of bounds
 */
#define MVGA_putPixelRGB(x, y, r, g, b)                                                                                                                                                                        \
    {                                                                                                                                                                                                       \
        uint32_t *address = (uint32_t *)(VGA_modeInfo->framebuffer + (y * VGA_modeInfo->pitch) + (x * VGA_bpp_in_bytes));                                                                                      \
        *address = (((r) & VGA_colorMask.r_mask) << VGA_modeInfo->red_position) | (((g) & VGA_colorMask.g_mask) << VGA_modeInfo->green_position) | (((b) & VGA_colorMask.b_mask) << VGA_modeInfo->blue_position); \
    }

/**
 * @brief Macro version of VGA_putPixel generally the a bit slower than MVGA_putPixelRGB due to memory costs of moving a structure but performs better than its function counterparts
 * 
 * @warning x and y are not checked for out of bounds
 */
#define MVGA_putPixel(x, y, color)                                                                                                                                                                        \
    {                                                                                                                                                                                                       \
        uint32_t *address = (uint32_t *)(VGA_modeInfo->framebuffer + (y * VGA_modeInfo->pitch) + (x * VGA_bpp_in_bytes));                                                                                      \
        *address = ((color##.r & VGA_colorMask.r_mask) << VGA_modeInfo->red_position) | ((color##.g & VGA_colorMask.g_mask) << VGA_modeInfo->green_position) | ((color##.b & VGA_colorMask.b_mask) << VGA_modeInfo->blue_position); \
    }

#endif // __VGA__
