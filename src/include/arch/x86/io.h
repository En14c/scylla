#ifndef __IO_H
#define __IO_H

#include <types.h>

#if defined(__X86_ARCH_TARGET)

#define io_inb(port)      \
    ({                    \
       u8_t __ret;        \
                          \
       __asm__ volatile   \
       (                  \
        "inb %w1, %0"     \
        : "=a" (__ret)    \
        : "d"  (port)     \
       );                 \
                          \
       __ret;             \
    })


#define io_inw(port)      \
    ({                    \
       u16_t __ret;       \
                          \
       __asm__ volatile   \
       (                  \
        "inw %w1, %0"     \
        : "=a" (__ret)    \
        : "d"  (port)     \
       );                 \
                          \
       __ret;             \
    })


#define io_outb(port, data)      \
    {                            \
       __asm__ volatile          \
       (                         \
        "outb %b0, %w1"          \
        :                        \
        : "a" (data), "d" (port) \
       );                        \
    }


#define io_outw(port, data)      \
    {                            \
       __asm__ volatile          \
       (                         \
        "outw %w0, %w1"          \
        :                        \
        : "a" (data), "d" (port) \
       );                        \
    }

#endif /* __X86_ARCH_TARGET */
#endif /* __IO_H */
