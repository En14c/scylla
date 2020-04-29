#include <drivers/video.h>

#include <sys/gdt.h>

void kmain(void)
{
    init_screen();
    init_gdt();
}
