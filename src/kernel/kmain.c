#include <sys/kernel.h>
#include <sys/gdt.h>
#include <sys/idt.h>
#include <sys/pic8259.h>
#include <sys/pit8253.h>

void kmain(void)
{
    init_screen();
    init_gdt();
    init_idt();
    pic8259_init();
    pit8253_init();
    sti();
    while(1);
}
