#include <sys/kernel.h>
#include <sys/gdt.h>
#include <sys/idt.h>
#include <sys/pic8259.h>
#include <sys/pit8253.h>
#include <sys/mm.h>
#include <multiboot.h>


void kmain(multiboot_info_t *mboot_info)
{
    init_screen();
    init_gdt();
    init_idt();
    pic8259_init();
    pit8253_init();
    sti();
    detect_physical_memory(mboot_info);
    while(1);
}
