#ifndef __GDT_H
#define __GDT_H

#include <compiler.h>
#include <types.h>


#define GDT_ENTRIES 0xff

/* gdt access bytes */
#define GDT_KERNEL_CS 0x9a
#define GDT_KERNEL_DS 0x92
#define GDT_USER_CS   0xfa
#define GDT_USER_DS   0xf2

/* gdt entries indexes */
#define GDT_NULL_INDX      0x0
#define GDT_KERNEL_CS_INDX 0x1
#define GDT_KERNEL_DS_INDX 0x2
#define GDT_USER_CS_INDX   0x3
#define GDT_USER_DS_INDX   0X4

/* kernel code/data segments selectors */
#define KERNEL_CS_SEL 0x08
#define KERNEL_DS_SEL 0x10
   
struct gdt_entry {
    u32_t limit_low: 16, base_low: 16;
    u32_t base_middle: 8, type: 4, s: 1, dpl: 2, p: 1,
          limit_high: 4, granularity: 4, base_high: 8;
} __attribute__((packed));


struct gdt_ptr {
    u16_t len;
    u32_t base;
}__attribute__((packed));


extern void init_gdt(void);


#endif /* __GDT_H */
