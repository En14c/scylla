#include <types.h>

#include <arch/x86/io.h>

#include <drivers/video.h>

#include <ascii.h>
#include <compiler.h>


/*
 * TODO: add mutex access for this struct
*/
struct video_mem {
    u16_t *base_addr;
    struct next_free_char_loc {
        u8_t x_cord;
        u8_t y_cord;
    } next_free_char_loc;
    struct char_color {
        u8_t frgrd_color;
        u8_t bkgrd_color;
    } char_color;
} video_mem = {
    .base_addr = (u16_t *)VGA_COLOR_TEXT_MODE_MEM_START_ADDR,
    .next_free_char_loc.x_cord = 0x0,
    .next_free_char_loc.y_cord = 0x0,
    .char_color.frgrd_color = VGA_CHAR_ATTR_COLOR_WHITE,
    .char_color.bkgrd_color = VGA_CHAR_ATTR_COLOR_BLACK,
};

/*
 * update the location of the hardware cursor
*/ 
static void _screen_update_cursor()
{
    /*
     * hardware cursor deals with the video memory as words (2 bytes each)
    */ 
    u16_t cursor_loc = 
        video_mem.next_free_char_loc.x_cord +
        video_mem.next_free_char_loc.y_cord * VGA_BUFFER_WIDTH;

#define HARDWARE_CURSOR_INDX_REG_PORT 0x3d4
#define HARDWARE_CURSOR_DATA_REG_PORT 0x3d5
#define HARDWARE_CURSOR_LOC_HIGH_CMD  0xe
#define HARDWARE_CURSOR_LOC_LOW_CMD   0xf
    /*
     * set the cursor location high byte
    */
    io_outb(HARDWARE_CURSOR_INDX_REG_PORT,
            HARDWARE_CURSOR_LOC_HIGH_CMD);
    io_outb(HARDWARE_CURSOR_DATA_REG_PORT,
            (cursor_loc & 0xff00) >> 0x8);
    /*
     * set the cursor location low byte
    */
    io_outb(HARDWARE_CURSOR_INDX_REG_PORT,
            HARDWARE_CURSOR_LOC_LOW_CMD);
    io_outb(HARDWARE_CURSOR_DATA_REG_PORT,
            cursor_loc & 0xff);
}

always_inline static void _screen_reset_cursor()
{
    video_mem.next_free_char_loc.x_cord =
        video_mem.next_free_char_loc.y_cord = 0;
    _screen_update_cursor();
}


/*
 * provides the imitation of screen scrolling by moving the 
 * VGA's frame buffer's lines up by one line
*/
static void _screen_scroll()
{
    u32_t fb_line_w;
    long *vidmem_ptr;

    /*
     * we fill the VGA's frame buffer's last line with a dummy 
     * character (blank char with dark foreground and background color)
    */
    u16_t dummy_char =
        VGA_GEN_CHAR(
                ASCII_SPACE_CHAR_CODE,
                video_mem.char_color.frgrd_color,
                video_mem.char_color.bkgrd_color);

    /*
     * move the screen up by one line only if we hit the last raw
     * in the frame buffer
    */
    if (video_mem.next_free_char_loc.y_cord >= VGA_BUFFER_HEIGHT) {
        // view the frame buffer as sizeof(long)bytes long words
        vidmem_ptr = (long *)video_mem.base_addr; 
        // num of sizeof(long)bytes long words in each frame buffer line
        fb_line_w = VGA_BUFFER_WIDTH * sizeof(u16_t) / sizeof(long);
        for (u32_t i = 0; i < fb_line_w * (VGA_BUFFER_HEIGHT - 1); i++)
            vidmem_ptr[i] =
                vidmem_ptr[i + fb_line_w];
        
        // fill last lines with the dummy character
        for (u32_t i = VGA_BUFFER_WIDTH * (VGA_BUFFER_HEIGHT - 1);
                i < VGA_BUFFER_WIDTH * VGA_BUFFER_HEIGHT; i++)
            video_mem.base_addr[i] = dummy_char;
        
        // set y_cord of the hw cursor to the last line in the frame buffer
        video_mem.next_free_char_loc.y_cord = VGA_BUFFER_HEIGHT - 1;
    }
}

/*
 * clear the screen by filling the frame buffer with space character
*/
static void _screen_clr()
{
    u16_t num_screen_chars;

    num_screen_chars = VGA_BUFFER_WIDTH * VGA_BUFFER_HEIGHT;
    
    for (u16_t i = 0; i < num_screen_chars; i++)
        video_mem.base_addr[i] =
            VGA_GEN_CHAR(
                ASCII_SPACE_CHAR_CODE,
                video_mem.char_color.frgrd_color,
                video_mem.char_color.bkgrd_color);

    _screen_reset_cursor();
}

/*
 * print a character to the screen
*/
static void _screen_prntchar(char _char)
{
    switch (_char) {
        // newline character
        case ASCII_NEWLINE_CHAR_CODE:
            video_mem.next_free_char_loc.x_cord = 0;
            video_mem.next_free_char_loc.y_cord++;
            break;
        // carriage return character
        case ASCII_CRETURN_CHAR_CODE:
            video_mem.next_free_char_loc.x_cord = 0;
            break;
        // backspace character
        case ASCII_BKSPACE_CHAR_CODE:
            if (video_mem.next_free_char_loc.x_cord)
                video_mem.next_free_char_loc.x_cord--;
            break;
        default:
            // printable characters
            if (_char >= ASCII_SPACE_CHAR_CODE) {
                u16_t *vid_mem_ptr =
                    video_mem.base_addr +
                    (video_mem.next_free_char_loc.x_cord +
                     video_mem.next_free_char_loc.y_cord * VGA_BUFFER_WIDTH);

                *vid_mem_ptr = VGA_GEN_CHAR(
                        _char, video_mem.char_color.frgrd_color,
                        video_mem.char_color.bkgrd_color);

                video_mem.next_free_char_loc.x_cord++;
            }
    }
    
    // end of the current line ?
    if (video_mem.next_free_char_loc.x_cord >= VGA_BUFFER_WIDTH) {
        video_mem.next_free_char_loc.x_cord = 0;
        video_mem.next_free_char_loc.y_cord++;
    }

    _screen_scroll();
    _screen_update_cursor();
}

/*
 * print a string to the screen
*/
static void _screen_prntstr(char *str)
{
    char _char;
    while ((_char = *str++))
        _screen_prntchar(_char);
}


always_inline void init_screen()
{
    _screen_clr();
}

always_inline void prntchar(char _char)
{
    _screen_prntchar(_char);
}

always_inline void prntstr(char *str)
{
    _screen_prntstr(str);
}
