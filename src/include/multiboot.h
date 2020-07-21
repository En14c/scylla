// https://www.gnu.org/software/grub/manual/multiboot/multiboot.html

#ifndef __MULTIBOOT_H
#define __MULTIBOOT_H

#include <types.h>

/* 
 * some flags we check in the flags field in multiboot information structure
*/
#define MULTIBOOT_FLAG_MEMSIZE 0x1
#define MULTIBOOT_FLAG_MMAP    (1 << 0x6)

#define MULTIBOOT_MMAP_MEM_AVAILABLE 0x1
#define MULTIBOOT_MMAP_MEM_RESERVED  0x2

typedef struct multiboot_memory_map
{
    u32_t size;
    u32_t base_addr_low, base_addr_high;
    u32_t length_low, length_high;
    u32_t type;
} __attribute((packed)) multiboot_memory_map_entry_t;


typedef struct multiboot_elf_section_header_table
{
    u32_t num;
    u32_t size;
    u32_t addr;
    u32_t shndx;
} __attribute__((packed)) multiboot_elf_section_header_table_t;


typedef struct multiboot_aout_symbol_table
{
    u32_t tabsize;
    u32_t strsize;
    u32_t addr;
    u32_t reserved;
} __attribute__((packed)) multiboot_aout_symbol_table_t;


typedef struct multiboot_info {
    u32_t flags;
    /* available memory from BIOS */
    u32_t memory_lower;
    u32_t memory_upper;
    /* root partition */
    u32_t boot_device;
    /* kernel command line */
    u32_t cmdline;
    /* boot module list */
    u32_t mods_count;
    u32_t mods_addr;
    /* symbol table informations */
    union {
        multiboot_elf_section_header_table_t elf_section_header_table;
        multiboot_aout_symbol_table_t aout_symbol_table;
    } symboltab_info;
    /* memory mapping buffer */
    u32_t mmap_length;
    u32_t mmap_addr;
    /* drive information buffer */
    u32_t drives_length;
    u32_t drives_addr;
    /* ROM configuration table */
    u32_t config_table;
    /* bootloader name */
    u32_t bootloader_name;
    /* Advanced Power Management table */
    u32_t apm_table;
    /* video info */
    u32_t vbe_control_info;
    u32_t vbe_mode_info;
    u16_t vbe_mode;
    u16_t vbe_interface_seg;
    u16_t vbe_interface_off;
    u16_t vbe_interface_len;
} __attribute__((packed)) multiboot_info_t;


#endif /* __MULTIBOOT_H */
