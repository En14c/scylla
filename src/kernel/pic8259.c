#include <compiler.h>
#include <arch/x86/io.h>
#include <sys/pic8259.h>


/*
 * set bitmask for master PIC
*/
always_inline void
pic8259_master_set_imr_bitmask(u8_t bitmask)
{
    io_outb(PIC_8259_MASTER_DATA_PORT, bitmask);
}

/*
 * set bitmask for slave PIC
*/
always_inline void
pic8259_slave_set_imr_bitmask(u8_t bitmask)
{
    io_outb(PIC_8259_SLAVE_DATA_PORT, bitmask);
}

/*
 * enable an IRQ line for master PIC
*/
void
pic8259_master_irqline_enable(u8_t irqline)
{
    u8_t bitmask;

    if (irqline >= PIC_8259_NUM_IRQ_LINES)
        goto _end_pic8259_master_irqline_enable;

    bitmask = io_inb(PIC_8259_MASTER_DATA_PORT) & ~(0x1 << irqline);
    io_outb(PIC_8259_MASTER_DATA_PORT, bitmask);

_end_pic8259_master_irqline_enable:
    return;
}
/*
 * disable an IRQ line for master PIC
*/
void
pic8259_master_irqline_disable(u8_t irqline)
{
    u8_t bitmask;

    if (irqline > PIC_8259_NUM_IRQ_LINES)
        goto _end_pic8259_master_irqline_disable;

    bitmask = io_inb(PIC_8259_MASTER_DATA_PORT) | (0x1 << irqline);
    io_outb(PIC_8259_MASTER_DATA_PORT, bitmask);

_end_pic8259_master_irqline_disable:
    return;
}

/*
 * enable an IRQ line for slave PIC
*/
void
pic8259_slave_irqline_enable(u8_t irqline)
{
    u8_t bitmask;

    if (irqline >= PIC_8259_NUM_IRQ_LINES)
        goto _end_pic8259_slave_irqline_enable;

    bitmask = io_inb(PIC_8259_SLAVE_DATA_PORT) & ~(0x1 << irqline);
    io_outb(PIC_8259_SLAVE_DATA_PORT, bitmask);

_end_pic8259_slave_irqline_enable:
    return;
}

/*
 * disable an IRQ line for slave PIC
*/
void
pic8259_slave_irqline_disable(u8_t irqline)
{
    u8_t bitmask;

    if (irqline >= PIC_8259_NUM_IRQ_LINES)
        goto _end_pic8259_slave_irqline_disable;

    bitmask = io_inb(PIC_8259_SLAVE_DATA_PORT) | (0x1 << irqline);
    io_outb(PIC_8259_SLAVE_DATA_PORT, bitmask);

_end_pic8259_slave_irqline_disable:
    return;
}

/* 
 * - reinitialize PIC 8259 master/slave controllers
 * - vector offset for master is reset to the value denoted by 
 *   PIC_8259_MASTER_ICW2 macro
 * - vector offset for slave is reset to the value denoted by
 *   PIC_8259_SLAVE_ICW2 macro
*/
void
pic8259_init(void)
{
    /* send ICW1 to start pic8259's initialization */
    io_outb(PIC_8259_MASTER_CMD_PORT, PIC_8259_MASTER_ICW1);
    io_outb(PIC_8259_SLAVE_CMD_PORT, PIC_8259_SLAVE_ICW1);
    /* send ICW2 to reset master pic's vector offset */
    io_outb(PIC_8259_MASTER_DATA_PORT, PIC_8259_MASTER_ICW2);
    /* send ICW2 to reset slave pic's vector offset */
    io_outb(PIC_8259_SLAVE_DATA_PORT, PIC_8259_SLAVE_ICW2);
    /* send ICW3 inform master/slave pic's of their connection on IRQ2 line */
    io_outb(PIC_8259_MASTER_DATA_PORT, PIC_8259_MASTER_ICW3);
    io_outb(PIC_8259_SLAVE_DATA_PORT, PIC_8259_SLAVE_ICW3);
    /* send ICW4 to set 80x86 mode */
    io_outb(PIC_8259_MASTER_DATA_PORT, PIC_8259_MASTER_ICW4);
    io_outb(PIC_8259_SLAVE_DATA_PORT, PIC_8259_SLAVE_ICW4);

    /* enable only IRQ line 0, disable all other IRQ lines */
    pic8259_master_set_imr_bitmask(PIC_8259_IMR_ENABLE_IRQ0);
    pic8259_slave_set_imr_bitmask(PIC_8259_IMR_DISABLE_ALL);
}
