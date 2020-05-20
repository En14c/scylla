#include <sys/pit8253.h>
#include <arch/x86/io.h>


void
pit8253_init(void)
{
    u32_t counter;

    counter = PIT_8253_INPUT_FREQ / PIT_8253_DEFAULT_OUT_FREQ;

    io_outb(PIT_8253_CMD_PORT, PIT_8253_REPEAT_MOD_CMD);
    io_outb(PIT_8253_CHANNEL0_DATA_PORT, (u8_t)(counter & 0xff));
    io_outb(PIT_8253_CHANNEL0_DATA_PORT, (u8_t)((counter >> 0x08) & 0xff));
}
