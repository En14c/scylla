#ifndef __VIDEO_H
#define __VIDEO_H

#include <types.h>


/*
 * start address of the VGA color text mode memory
*/
#define VGA_COLOR_TEXT_MODE_MEM_START_ADDR  0xc00b8000


/*
 * width and height of the screen
*/
enum {
    VGA_BUFFER_WIDTH   = 80,
    VGA_BUFFER_HEIGHT  = 25
};


/*
 * define the color constants used in the attribute
 * byte of the VGA's framebuffer's characters
*/
enum {
    VGA_CHAR_ATTR_COLOR_BLACK            = 0x0,
    VGA_CHAR_ATTR_COLOR_BLUE             = 0x1,
    VGA_CHAR_ATTR_COLOR_GREEN            = 0x2,
    VGA_CHAR_ATTR_COLOR_CYAN             = 0x3,
    VGA_CHAR_ATTR_COLOR_RED              = 0x4,
    VGA_CHAR_ATTR_COLOR_MAGENTA          = 0x5,
    VGA_CHAR_ATTR_COLOR_BROWN            = 0x6,
    VGA_CHAR_ATTR_COLOR_GREY             = 0x7,
    VGA_CHAR_ATTR_COLOR_DARK_GREY        = 0x8,
    VGA_CHAR_ATTR_COLOR_LIGHT_BLUE       = 0x9,
    VGA_CHAR_ATTR_COLOR_LIGHT_GREEN      = 0xa,
    VGA_CHAR_ATTR_COLOR_LIGHT_CYAN       = 0xb,
    VGA_CHAR_ATTR_COLOR_LIGHT_RED        = 0xc,
    VGA_CHAR_ATTR_COLOR_LIGHT_MAGENTA    = 0xd,
    VGA_CHAR_ATTR_COLOR_YELLOW           = 0xe,
    VGA_CHAR_ATTR_COLOR_WHITE            = 0xf,
};


/*
 * generating VGA's frame buffer's character 
 * set the char attribute (foreground color, background color)
 * and the char code 
*/
#define VGA_GEN_CHAR(__chr, __frgrd_clr, __bkgrd_clr)                    \
    ({                                                                   \
        u8_t __char_attr =                                               \
            ((u8_t)(__frgrd_clr) & 0x0f) | ((u8_t)(__bkgrd_clr) << 0x4); \
        u16_t __char = (__char_attr << 0x8) | (u8_t)(__chr);             \
        __char;                                                          \
    })


/*
 * functions prototypes
*/
extern void init_screen(void);
extern void prntstr(char *);
extern void prntchar(char);


#endif /* __VIDEO_H */
