extern kmain

global _kstart


; include information about available memory in the mutliboot
; information structure
MULTIBOOTHEADER_FLAG_MEMINFO equ 0x01 << 0x01 
MULTIBOOTHEADER_MAGIC_NUMBER equ 0x1badb002
MULTIBOOTHEADER_FLAGS        equ MULTIBOOTHEADER_FLAG_MEMINFO 
MULTIBOOTHEADER_CHECKSUM     equ -(MULTIBOOTHEADER_MAGIC_NUMBER + MULTIBOOTHEADER_FLAGS)

KERNEL_VIRT_LOAD_ADDRESS equ 0xc0000000
PAGE_SIZE equ 0x1000
PAGE_TABLE_ENTRYS_NUM equ 0x400

; page table entry's flags
PTE_FLAG_PRESENT    equ 0x01
PTE_FLAG_READ_WRITE equ 0x02

section .multi_boot_header
; multiboot header must be 32 bit aligned
align 4
    
    dd MULTIBOOTHEADER_MAGIC_NUMBER
    dd MULTIBOOTHEADER_FLAGS
    dd MULTIBOOTHEADER_CHECKSUM

section .data

; setup paging
; the addresses of page tables and page directory must be page size aligned

align 0x1000

page_table_0 : times 0x1000 db 0x0
page_table_768 : times 0x1000 db 0x0

early_page_directory:
    dd ((page_table_0 - KERNEL_VIRT_LOAD_ADDRESS) + 0x03)
    times 0x2ff dd 0x0
    dd ((page_table_768 - KERNEL_VIRT_LOAD_ADDRESS) + 0x03)
    times 0xff dd 0x0

section .text

; identity mapping the first page table
fill_page_table_0:
    xor ecx, ecx
    xor ebx, ebx
    xor eax, eax
    xor edx, edx
    mov edx, (page_table_0 - KERNEL_VIRT_LOAD_ADDRESS)
fill_loop_0:
    mov eax, ebx
    or  eax, PTE_FLAG_PRESENT | PTE_FLAG_READ_WRITE
    mov [edx + ecx * 4], eax
    add ebx, PAGE_SIZE
    inc ecx
    cmp ecx, PAGE_TABLE_ENTRYS_NUM
    je  end_fill_loop_0
    jmp fill_loop_0
end_fill_loop_0:
    ret

; map the virtual address space's 4MB starting @ 0xc0000000
; to the first 4M in the physical memory
fill_page_table_768:
    xor eax, eax
    xor ebx, ebx
    xor ecx, ecx
    xor edx, edx
    mov edx, (page_table_768 - KERNEL_VIRT_LOAD_ADDRESS)
fill_loop_1:
    mov eax, ebx
    or  eax, PTE_FLAG_PRESENT | PTE_FLAG_READ_WRITE
    mov [edx + ecx * 4], eax
    add ebx, PAGE_SIZE
    inc ecx
    cmp ecx, PAGE_TABLE_ENTRYS_NUM
    je  end_fill_loop_1
    jmp fill_loop_1
end_fill_loop_1:
    ret
 
load_early_page_directory:
    mov eax, (early_page_directory - KERNEL_VIRT_LOAD_ADDRESS)
    mov cr3, eax
    ret

enable_paging:
    mov eax, cr0
    or  eax, 0x80000000
    mov cr0, eax
    ret

_kstart:
    ; save grub's multiboot information table pointer
    push ebx
    call fill_page_table_0
    call fill_page_table_768
    call load_early_page_directory
    call enable_paging
    ; restor grub's multiboot information table pointer
    pop  ebx
    mov  eax, _kernel_higher_half_start
    jmp  eax

_kernel_higher_half_start:
    ; esp points to the bottom of the stack (high address)
    lea esp, [ kernelstack + KERNSTACK_SIZE ]
    ; pass grub's multiboot information table pointer to kernel's main function
    push ebx
    call kmain
    hlt

; @ make sure esp points to a region of memory that is part of out kernel.
; @ GRUB understands ELF so it will allocate the amount of bytes reserved here.
KERNSTACK_SIZE equ 0x1000

section .bss
align 4
kernelstack:
    resb KERNSTACK_SIZE
