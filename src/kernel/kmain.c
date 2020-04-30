#include <drivers/video.h>
#include <sys/gdt.h>
#include <sys/idt.h>

void kmain(void)
{
    init_screen();
    init_gdt();
    init_idt();
}
