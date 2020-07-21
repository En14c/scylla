#ifndef __KERNEL_H
#define __KERNEL_H

/* halt the system */
#define hlt()          \
    __asm__ (          \
            "cli \t\n" \
            "hlt"      \
            );

/* enable interrupts */
#define sti() __asm__ ("sti")

#endif /* __KERNEL_H */
