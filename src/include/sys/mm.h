#ifndef __MEMORY_MANAGEMENT_H
#define __MEMORY_MANAGEMENT_H

#include <types.h>
#include <printk.h>
#include <stddef.h>
#include <multiboot.h>
#include <sys/kernel.h>


typedef struct physical_memory_info
{
    /* kernel's physical memory start address (load address) */
    u32_t kernel_load_address;
    /* kernel's physical memory end address (adding the bitmap's size) */
    u32_t kernel_end_address;
    /* overall size of the available RAM areas */
    u32_t size;
    /* the number of physical RAM frames */
    u32_t frames_num;
    /* physical frame's size in bytes */
    u32_t frame_size;
    /* bitmap for tracking the status of the physical frames (USED || FREE) */
#define PHYSICAL_MEM_FRAMES_BITMAP_SLOT_SIZE 0x8
    u32_t frames_bitmap_size;
    char *frames_bitmap;
} __attribute__((packed)) physical_memory_info_t;

typedef u32_t pde_t;
typedef u32_t pte_t;

#define MM_PAGING_INIT_FATAL_ERROR(errmsg) \
    do {                                   \
        printk(errmsg);                    \
        hlt();                             \
    } while (0)

#define pdir_switch(pdir_phys_addr) \
    __asm__ (                       \
        "movl %0, %%eax     \t\n"   \
        "movl %%eax, %%cr3  \t\n"   \
        :: "m" ((pdir_phys_addr))   \
    )

#define PHYSICAL_FRAMES_NUM         0x8000
#define PHYSICAL_FRAMES_BITMAP_SIZE (PHYSICAL_FRAMES_NUM / 8)

#define PHYSICAL_FRAME_USED 0x1
#define PHYSICAL_FRAME_FREE !PHYSICAL_FRAME_USED

#define NO_FREE_PHYSICAL_FRAMES_ERROR ~0x0

#define PHYSICAL_FRAME_MARK_USED_SUCCESS 0x0
#define PHYSICAL_FRAME_MARK_USED_ERROR   ~PHYSICAL_FRAME_MARK_USED_SUCCESS

#define PHYSICAL_FRAME_ALLOCATE_SUCCESS 0x0
#define PHYSICAL_FRAME_ALLOCATE_ERROR   ~PHYSICAL_FRAME_ALLOCATE_SUCCESS

#define PHYSICAL_FRAME_DEALLOCATE_SUCCESS 0x0
#define PHYSICAL_FRAME_DEALLOCATE_ERROR   ~PHYSICAL_FRAME_DEALLOCATE_SUCCESS

#define PHYSICAL_FRAME_MARK_FREE_SUCCESS 0x0
#define PHYSICAL_FRAME_MARK_FREE_ERROR   ~PHYSICAL_FRAME_MARK_FREE_SUCCESS

#define VIRTUAL_MEM_PAGE_ALLOCATION_SUCCESS 0x0
#define VIRTUAL_MEM_PAGE_ALLOCATION_ERROR   ~VIRTUAL_MEM_PAGE_ALLOCATION_SUCCESS

#define MM_VIRTUAL_ADDRESS_MAPPING_SUCCESS 0x0
#define MM_VIRTUAL_ADDRESS_MAPPING_ERROR   ~MM_VIRTUAL_ADDRESS_MAPPING_SUCCESS

#define MM_VIRTUAL_ADDRESS_UNMAPPING_SUCCESS 0x0
#define MM_VIRTUAL_ADDRESS_UNMAPPING_ERROR   ~MM_VIRTUAL_ADDRESS_UNMAPPING_SUCCESS

#define INVALID_PDIR_ENTRY   0x0
#define INVALID_PTABLE_ENTRY INVALID_PDIR_ENTRY

#define PHYSICAL_MEM_FRAME_SIZE       0x1000
#define PHYSICAL_MEM_ADDR_FRAME_ALIGN ~(PHYSICAL_MEM_FRAME_SIZE - 1)

#define MM_PAGE_SIZE  0x1000
#define MM_FRAME_SIZE MM_PAGE_SIZE

#define MM_PAGE_OFFSET_BITS_NUM  0x0c
#define MM_FRAME_OFFSET_BITS_NUM MM_PAGE_OFFSET_BITS_NUM

#define MM_KERNEL_VIRTUAL_LOAD_ADDRESS 0xc0000000

#define MM_GET_PHYSICAL_MEM_FRAME_NUM(phys_addr) \
    (((u32_t)(phys_addr)) >> MM_FRAME_OFFSET_BITS_NUM)

#define MM_VIRTADDR_ROUND_PAGE_UP(virt_addr) ((virt_addr) & ~0xfff)
#define MM_GET_PTABLE_INDEX(virt_addr) (((virt_addr) >> 12) & 0x3ff)
#define MM_GET_PDIR_INDEX(virt_addr) (((virt_addr) >> 22) & 0x3ff)

#define MM_PTABLE_ENTRY_FLAG_PRESENT        0x01
#define MM_PTABLE_ENTRY_FLAG_WRITABLE       0x02
#define MM_PTABLE_ENTRY_FLAG_ACCESS_LEVEL   0x04
#define MM_PTABLE_ENTRY_FLAG_WRITE_THROUGH  0x08
#define MM_PTABLE_ENTRY_FLAG_CACHE_DISABLED 0x10

#define MM_PDIR_ENTRY_FLAG_PRESENT        0x01
#define MM_PDIR_ENTRY_FLAG_WRITABLE       0x02
#define MM_PDIR_ENTRY_FLAG_ACCESS_LEVEL   0x04
#define MM_PDIR_ENTRY_FLAG_WRITE_THROUGH  0x08
#define MM_PDIR_ENTRY_FLAG_CACHE_DISABLED 0x10
#define MM_PDIR_ENTRY_FLAG_PAGE_SIZE      0x80

/* functions' prototypes */
void detect_physical_memory(multiboot_info_t *);
u32_t allocate_physical_memory_frame(void);
u32_t deallocate_physical_memory_frame(u32_t);
void init_paging(void);
u32_t map_virtual_address(u32_t, u32_t);
u32_t unmap_virtual_address(u32_t);

#endif /* __MEMORY_MANAGEMENT_H */
