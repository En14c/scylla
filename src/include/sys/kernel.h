#ifndef __KERNEL_H
#define __KERNEL_H

/* halt the system */
#define hlt()      \
    __asm__ (      \
        "cli \t\n" \
        "hlt"      \
    );

/* enable interrupts */
#define sti() __asm__ ("sti")

#define __equal__(x, y) ((u32_t)(x) == (u32_t)(y))
#define __not_equal__(x, y) ((u32_t)(x) != (u32_t)(y))

#endif /* __KERNEL_H */
