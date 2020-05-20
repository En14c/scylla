#ifndef __PIC_8259
#define __PIC_8259

/* # IRQ lines per PIC 8259 IC */
#define PIC_8259_NUM_IRQ_LINES 0x08

#define PIC_8259_MASTER_CMD_PORT  0x20
#define PIC_8259_MASTER_DATA_PORT 0x21
#define PIC_8259_SLAVE_CMD_PORT   0xa0
#define PIC_8259_SLAVE_DATA_PORT  0xa1

/* initialization control/command words */
#define PIC_8259_MASTER_ICW1 0x11 /* (cascading mode | ICW4 needed) */
#define PIC_8259_SLAVE_ICW1  PIC_8259_MASTER_ICW1
#define PIC_8259_MASTER_ICW2 0x20 /* master's vector offset */
#define PIC_8259_SLAVE_ICW2  0x28 /* slave's vector offset */
#define PIC_8259_MASTER_ICW3 0x04 /* master connected to slave /w IRQ2 line */
#define PIC_8259_SLAVE_ICW3  0x02 /* salve connected to master /w IRQ2 line */
#define PIC_8259_MASTER_ICW4 0x01 /* 80x86 mode */
#define PIC_8259_SLAVE_ICW4  PIC_8259_MASTER_ICW4

/*
 * IRQs line number should be refrenced /w these macros for more clarity
*/
#define PIC_8259_IRQ_LINE0 0x00
#define PIC_8259_IRQ_LINE1 0x01
#define PIC_8259_IRQ_LINE2 0x02
#define PIC_8259_IRQ_LINE3 0x03
#define PIC_8259_IRQ_LINE4 0x04
#define PIC_8259_IRQ_LINE5 0x05
#define PIC_8259_IRQ_LINE6 0x06
#define PIC_8259_IRQ_LINE7 0x07

/* indexs of IRQs handlers in IDT */
#define IDT_IRQ0_INDX  PIC_8259_MASTER_ICW2 + 0x00
#define IDT_IRQ1_INDX  PIC_8259_MASTER_ICW2 + 0x01
#define IDT_IRQ2_INDX  PIC_8259_MASTER_ICW2 + 0x02
#define IDT_IRQ3_INDX  PIC_8259_MASTER_ICW2 + 0x03
#define IDT_IRQ4_INDX  PIC_8259_MASTER_ICW2 + 0x04
#define IDT_IRQ5_INDX  PIC_8259_MASTER_ICW2 + 0x05
#define IDT_IRQ6_INDX  PIC_8259_MASTER_ICW2 + 0x06
#define IDT_IRQ7_INDX  PIC_8259_MASTER_ICW2 + 0x07
#define IDT_IRQ8_INDX  PIC_8259_SLAVE_ICW2  + 0x00
#define IDT_IRQ9_INDX  PIC_8259_SLAVE_ICW2  + 0x01
#define IDT_IRQ10_INDX PIC_8259_SLAVE_ICW2  + 0x02
#define IDT_IRQ11_INDX PIC_8259_SLAVE_ICW2  + 0x03
#define IDT_IRQ12_INDX PIC_8259_SLAVE_ICW2  + 0x04
#define IDT_IRQ13_INDX PIC_8259_SLAVE_ICW2  + 0x05
#define IDT_IRQ14_INDX PIC_8259_SLAVE_ICW2  + 0x06
#define IDT_IRQ15_INDX PIC_8259_SLAVE_ICW2  + 0x07

/*
 * IRQ lines enable constants to be used /w the PIC8259 helper functions
 * that generates and/or sets the bitmask for PIC8259 master/slave's IMR
*/
#define PIC_8259_IMR_ENABLE_ALL  (0x00)
#define PIC_8259_IMR_DISABLE_ALL (0xff)
#define PIC_8259_IMR_ENABLE_IRQ0 (~(0x01 << 0x00))
#define PIC_8259_IMR_ENABLE_IRQ1 (~(0x01 << 0x01))
#define PIC_8259_IMR_ENABLE_IRQ2 (~(0x01 << 0x02))
#define PIC_8259_IMR_ENABLE_IRQ3 (~(0x01 << 0x03))
#define PIC_8259_IMR_ENABLE_IRQ4 (~(0x01 << 0x04))
#define PIC_8259_IMR_ENABLE_IRQ5 (~(0x01 << 0x05))
#define PIC_8259_IMR_ENABLE_IRQ6 (~(0x01 << 0x06))
#define PIC_8259_IMR_ENABLE_IRQ7 (~(0x01 << 0x07))


/* defined in isr.s */
void _irq0(void);
extern void _irq1(void);
extern void _irq2(void);
extern void _irq3(void);
extern void _irq4(void);
extern void _irq5(void);
extern void _irq6(void);
extern void _irq7(void);
extern void _irq8(void);
extern void _irq9(void);
extern void _irq10(void);
extern void _irq11(void);
extern void _irq12(void);
extern void _irq13(void);
extern void _irq14(void);
extern void _irq15(void);

extern void pic8259_master_irqline_enable(u8_t);
extern void pic8259_master_irqline_disable(u8_t);
extern void pic8259_slave_irqline_enable(u8_t);
extern void pic8259_slave_irqline_disable(u8_t);
extern void pic8259_master_set_intrpt_bitmask(u8_t);
extern void pic8259_slave_set_intrpt_bitmask(u8_t);
extern void pic8259_init(void);

#endif /* __PIC_8259 */
