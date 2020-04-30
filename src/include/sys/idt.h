#ifndef __IDT_H
#define __IDT_H


/* num of IDT entries */
#define IDT_ENTRIES 0xff

/* 
 * IDT descriptor types (/w DPL: 0)
*/
#define IDT_INTERRUPT_GATE_DPL0 0x8e
#define IDT_TRAP_GATE_DPL0      0x8f
#define IDT_TASK_GATE_DPL0      0x85


/*
 * IDT entries indexes
 * NOTE: we only define the ones we have a specific handler for
*/
#define IDT_CPU_DIV_ZERO_INDX        0x00
#define IDT_CPU_NMSKABLE_INTRPT_INDX 0x02
#define IDT_CPU_BRKPOINT_INDX        0x03
#define IDT_CPU_OVERFLOW_INDX        0x04
#define IDT_CPU_INVALID_OPCODE_INDX  0X06
#define IDT_CPU_DOUBLE_FAULT_INDX    0x08
#define IDT_CPU_INVALID_TSS_INDX     0x0a
#define IDT_CPU_SEG_NPRSNT_INDX      0x0b
#define IDT_CPU_STACK_SEG_FAULT_INDX 0x0c
#define IDT_CPU_GPF_INDX             0x0d


struct idt_entry {
    u32_t offset_low: 16, sel: 16;
    u32_t always0: 8, flags: 8, offset_high: 16; 
} __attribute__((packed));

struct idt_ptr {
    u16_t len;
    u32_t base;
} __attribute__((packed));

/* definition in src/kernel/idt.c */
extern void init_idt(void);

/* definitions in src/kernel/isr.s */
extern void _cpu_div_zero (void);
extern void _cpu_nmskable_intrpt (void);
extern void _cpu_brkpoint (void);
extern void _cpu_overflow (void);
extern void _cpu_invalid_opcode (void);
extern void _cpu_double_fault (void);
extern void _cpu_invalid_tss (void);
extern void _cpu_segment_not_prsnt (void);
extern void _cpu_stack_segment_fault (void);
extern void _cpu_general_protection_fault (void);

#endif /* __IDT_H */
