extern kmain

global _kstart


MULTIBOOTHEADER_FLAG_MEMINFO equ 1 << 1 ; include information about available memory in the multiboot information structure 
MULTIBOOTHEADER_MAGIC_NUMBER equ 0x1badb002
MULTIBOOTHEADER_FLAGS        equ MULTIBOOTHEADER_FLAG_MEMINFO 
MULTIBOOTHEADER_CHECKSUM     equ -(MULTIBOOTHEADER_MAGIC_NUMBER + MULTIBOOTHEADER_FLAGS)

section .multi_boot_header
align 4 ; multiboot header must be 32bit aligned
    
    dd MULTIBOOTHEADER_MAGIC_NUMBER
    dd MULTIBOOTHEADER_FLAGS
    dd MULTIBOOTHEADER_CHECKSUM

section .text

_kstart:
    lea esp, [ kernelstack + KERNSTACK_SIZE ]  ; esp points to the bottom of the stack (high address)
    
    push ebx    ;multiboot info
    call kmain
    hlt

; @ make sure esp points to a region of memory that is part of out kernel.
; @ GRUB understands ELF so it will allocate the amount of bytes reserved here.
KERNSTACK_SIZE equ 0x1000

section .bss
align 4
kernelstack:
    resb KERNSTACK_SIZE
