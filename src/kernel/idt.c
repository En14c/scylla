#include <compiler.h>
#include <types.h>
#include <string.h>
#include <sys/gdt.h>
#include <sys/idt.h>
#include <sys/interrupt.h>


static struct idt_entry idt[IDT_ENTRIES];
static struct idt_ptr idt_ptr;


always_inline
static void
set_idt_entry (u32_t indx, u32_t offset, u16_t sel, u8_t flags)
{
    idt[indx].offset_low  = offset & 0xffff;
    idt[indx].offset_high = (offset >> 16) & 0xffff;
    idt[indx].sel         = sel;
    idt[indx].always0     = 0x0;
    idt[indx].flags       = flags;
}

void
init_idt (void)
{
    /*
     * set a default interrupt handler
    */
    for (u32_t indx = 0; indx < IDT_ENTRIES; indx++)
        set_idt_entry(
            indx, (u32_t)default_interrupt_handler, KERNEL_CS_SEL,
            IDT_INTERRUPT_GATE_DPL0);

    set_idt_entry(
            IDT_CPU_DIV_ZERO_INDX, (u32_t)_cpu_div_zero,
            KERNEL_CS_SEL, IDT_INTERRUPT_GATE_DPL0);
    set_idt_entry(
            IDT_CPU_NMSKABLE_INTRPT_INDX, (u32_t)_cpu_nmskable_intrpt,
            KERNEL_CS_SEL, IDT_INTERRUPT_GATE_DPL0);
    set_idt_entry(
            IDT_CPU_BRKPOINT_INDX, (u32_t)_cpu_brkpoint,
            KERNEL_CS_SEL, IDT_INTERRUPT_GATE_DPL0);
    set_idt_entry(
            IDT_CPU_OVERFLOW_INDX, (u32_t)_cpu_overflow,
            KERNEL_CS_SEL, IDT_INTERRUPT_GATE_DPL0);
    set_idt_entry(
            IDT_CPU_INVALID_OPCODE_INDX, (u32_t)_cpu_invalid_opcode,
            KERNEL_CS_SEL, IDT_INTERRUPT_GATE_DPL0);
    set_idt_entry(
            IDT_CPU_DOUBLE_FAULT_INDX, (u32_t)_cpu_double_fault,
            KERNEL_CS_SEL, IDT_INTERRUPT_GATE_DPL0);
    set_idt_entry(
            IDT_CPU_INVALID_TSS_INDX, (u32_t)_cpu_invalid_tss,
            KERNEL_CS_SEL, IDT_INTERRUPT_GATE_DPL0);
    set_idt_entry(
            IDT_CPU_SEG_NPRSNT_INDX, (u32_t)_cpu_segment_not_prsnt,
            KERNEL_CS_SEL, IDT_INTERRUPT_GATE_DPL0);
    set_idt_entry(
            IDT_CPU_STACK_SEG_FAULT_INDX, (u32_t)_cpu_stack_segment_fault,
            KERNEL_CS_SEL, IDT_INTERRUPT_GATE_DPL0);
    set_idt_entry(
            IDT_CPU_GPF_INDX, (u32_t)_cpu_general_protection_fault,
            KERNEL_CS_SEL, IDT_INTERRUPT_GATE_DPL0);

    idt_ptr.len  = (sizeof(struct idt_entry) * IDT_ENTRIES) - 1;
    idt_ptr.base = (u32_t)&idt;

    /* reload the idt register */
    __asm__ volatile
        (
         "leal  %0, %%eax \n"
         "lidtl (%%eax)   \n"
         :
         : "m" (idt_ptr)
        );
}

