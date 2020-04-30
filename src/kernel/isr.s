%define __KERNEL_DS 0x10 ; kernel data segment selector


; processor exceptions

extern handle_cpu_div_zero
extern handle_cpu_nmskable_intrpt
extern handle_cpu_brkpoint
extern handle_cpu_overflow
extern handle_cpu_invalid_opcode
extern handle_cpu_double_fault
extern handle_cpu_invalid_tss
extern handle_cpu_segment_not_prsnt
extern handle_cpu_stack_segment_fault
extern handle_cpu_general_protection_fault

global _cpu_div_zero
global _cpu_nmskable_intrpt
global _cpu_brkpoint
global _cpu_overflow
global _cpu_invalid_opcode
global _cpu_double_fault
global _cpu_invalid_tss
global _cpu_segment_not_prsnt
global _cpu_stack_segment_fault
global _cpu_general_protection_fault


section .text

; cpu divide by zero exception
_cpu_div_zero:
    cli
    pusha
    push  ebx
    mov   bx, __KERNEL_DS
    mov   ds, bx
    mov   gs, bx
    mov   es, bx
    mov   fs, bx
    pop   ebx
    push  ds
    push  gs
    push  fs
    push  es
    call  handle_cpu_div_zero
    pop   es
    pop   fs
    pop   gs
    pop   ds
    popa
    sti
    iret

; cpu non maskable interrupt exception
_cpu_nmskable_intrpt:
    cli
    pusha
    push  ebx
    mov   bx, __KERNEL_DS
    mov   ds, bx
    mov   gs, bx
    mov   es, bx
    mov   fs, bx
    pop   ebx
    push  ds
    push  gs
    push  fs
    push  es
    call  handle_cpu_nmskable_intrpt
    pop   es
    pop   fs
    pop   gs
    pop   ds
    popa
    sti
    iret

; cpu break point exception
_cpu_brkpoint:
    cli
    pusha
    push  ebx
    mov   bx, __KERNEL_DS
    mov   ds, bx
    mov   gs, bx
    mov   es, bx
    mov   fs, bx
    pop   ebx
    push  ds
    push  gs
    push  fs
    push  es
    call  handle_cpu_brkpoint
    pop   es
    pop   fs
    pop   gs
    pop   ds
    popa
    sti
    iret

; cpu overflow exception
_cpu_overflow:
    cli
    pusha
    push  ebx
    mov   bx, __KERNEL_DS
    mov   ds, bx
    mov   gs, bx
    mov   es, bx
    mov   fs, bx
    pop   ebx
    push  ds
    push  gs
    push  fs
    push  es
    call  handle_cpu_overflow
    pop   es
    pop   fs
    pop   gs
    pop   ds
    popa
    sti
    iret

; cpu invalid opcode
_cpu_invalid_opcode:
    cli
    pusha
    push  ebx
    mov   bx, __KERNEL_DS
    mov   ds, bx
    mov   gs, bx
    mov   es, bx
    mov   fs, bx
    pop   ebx
    push  ds
    push  gs
    push  fs
    push  es
    call  handle_cpu_invalid_opcode
    pop   es
    pop   fs
    pop   gs
    pop   ds
    popa
    sti
    iret

; cpu double fault exception
_cpu_double_fault:
    cli
    pusha
    push  ebx
    mov   bx, __KERNEL_DS
    mov   ds, bx
    mov   gs, bx
    mov   es, bx
    mov   fs, bx
    pop   ebx
    push  ds
    push  gs
    push  fs
    push  es
    call  handle_cpu_double_fault
    pop   es
    pop   fs
    pop   gs
    pop   ds
    popa
    add   esp, 0x4 ; remove error code from kernel stack
    sti
    iret

; cpu invalid tss exception
_cpu_invalid_tss:
    cli
    pusha
    push  ebx
    mov   bx, __KERNEL_DS
    mov   ds, bx
    mov   gs, bx
    mov   es, bx
    mov   fs, bx
    pop   ebx
    push  ds
    push  gs
    push  fs
    push  es
    call  handle_cpu_invalid_tss
    pop   es
    pop   fs
    pop   gs
    pop   ds
    popa
    add   esp, 0x4
    sti
    iret

; cpu segment not present exception
_cpu_segment_not_prsnt:
    cli
    pusha
    push  ebx
    mov   bx, __KERNEL_DS
    mov   ds, bx
    mov   gs, bx
    mov   es, bx
    mov   fs, bx
    pop   ebx
    push  ds
    push  gs
    push  fs
    push  es
    call  handle_cpu_segment_not_prsnt
    pop   es
    pop   fs
    pop   gs
    pop   ds
    popa
    add   esp, 0x4
    sti
    iret

; cpu stack segment fault exception
_cpu_stack_segment_fault:
    cli
    pusha
    push  ebx
    mov   bx, __KERNEL_DS
    mov   ds, bx
    mov   gs, bx
    mov   es, bx
    mov   fs, bx
    pop   ebx
    push  ds
    push  gs
    push  fs
    push  es
    call  handle_cpu_stack_segment_fault
    pop   es
    pop   fs
    pop   gs
    pop   ds
    popa
    add   esp, 0x4
    sti
    iret

; cpu general protection fault exception
_cpu_general_protection_fault:
    cli
    pusha
    push  ebx
    mov   bx, __KERNEL_DS
    mov   ds, bx
    mov   gs, bx
    mov   es, bx
    mov   fs, bx
    pop   ebx
    push  ds
    push  gs
    push  fs
    push  es
    call  handle_cpu_general_protection_fault
    pop   es
    pop   fs
    pop   gs
    pop   ds
    popa
    add   esp, 0x4
    sti
    iret
