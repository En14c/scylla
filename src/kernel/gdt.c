#include <compiler.h>
#include <types.h>
#include <sys/gdt.h>


static struct gdt_entry gdt[GDT_ENTRIES];
static struct gdt_ptr gdt_ptr;


always_inline
static void
set_gdt_entry (
        u32_t indx, u32_t base_address, u32_t limit,
        u32_t access_flags, u32_t granularity)
{
    gdt[indx].limit_low   = limit & 0xffff;
    gdt[indx].base_low    = base_address & 0xffff;
    gdt[indx].base_middle = (base_address >> 16) & 0xff;
    gdt[indx].type        = access_flags & 0x0f;
    gdt[indx].s           = (access_flags & 0x10) >> 0x4;
    gdt[indx].dpl         = (access_flags & 0x60) >> 0x5;
    gdt[indx].p           = (access_flags & 0x80) >> 0x7;
    gdt[indx].limit_high  = (limit >> 16) & 0x0f;
    gdt[indx].granularity = granularity;
    gdt[indx].base_high   = (base_address >> 24) & 0xff;
}


void
init_gdt (void)
{
    // NULL segment to make the cpu happy
    set_gdt_entry(GDT_NULL_INDX, 0x0, 0x0, 0x0, 0x0);
    
    set_gdt_entry(
            GDT_KERNEL_CS_INDX, 0x0, 0xffffffff, GDT_KERNEL_CS, 0x0c);
    set_gdt_entry(
            GDT_KERNEL_DS_INDX, 0x0, 0xffffffff, GDT_KERNEL_DS, 0x0c);
    
    set_gdt_entry(
            GDT_USER_CS_INDX, 0x0, 0xffffffff, GDT_USER_CS, 0x0c);
    set_gdt_entry(
            GDT_USER_DS_INDX, 0x0, 0xffffffff, GDT_USER_DS, 0x0c);


    gdt_ptr.len   = (sizeof(struct gdt_entry) * GDT_ENTRIES) - 1;
    gdt_ptr.base  = (u32_t)&gdt;

    /* reload the gdt register and the segment selectors */
    __asm__ volatile
        (
         "leal  %0, %%eax      \n"
         "lgdtl (%%eax)        \n"
         "movw  $0x10, %%ax    \n"
         "movw  %%ax, %%ds     \n"
         "movw  %%ax, %%es     \n"
         "movw  %%ax, %%fs     \n"
         "movw  %%ax, %%gs     \n"
         "movw  %%ax, %%ss     \n"
         "ljmp  $0x08, $.flush \n"
         ".flush:"
         : 
         : "m" (gdt_ptr)
        );
}
