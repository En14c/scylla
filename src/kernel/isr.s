%define __KERNEL_DS 0x10 ; kernel data segment selector

%define __PIC8259_MASTER_CMD_PORT  0x20
%define __PIC8259_MASTER_DATA_PORT 0x21
%define __PIC8259_SLAVE_CMD_PORT   0xa0
%define __PIC8259_SLAVE_DATA_PORT  0xa1
%define __PIC8259_EOI_CMD          0x20
%define __PIC8259_ISR_READ_CMD     0x0a ; read ISR on next read

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


; IRQS
extern handle_irq0
extern handle_irq1
extern handle_irq3
extern handle_irq4
extern handle_irq5
extern handle_irq6
extern handle_irq7
extern handle_irq8
extern handle_irq9
extern handle_irq10
extern handle_irq11
extern handle_irq12
extern handle_irq13
extern handle_irq14
extern handle_irq15

global _irq0  ; programmable interval timer interrupt
global _irq1  ; keyboard interrupt
global _irq2  ; cascade interrupt never raised used internally by master/slave PICs
global _irq3  ; serial port 2 interrupt
global _irq4  ; serial port 1 interrupt
global _irq5  ; parallel port 2 interrupt
global _irq6  ; floppy disk interrupt
global _irq7  ; parallel port 1/spurious interrupt
global _irq8  ; CMOS real time clock interrupt
global _irq9  ; * free for peripherals *
global _irq10 ; * free for peripherals *
global _irq11 ; * free for peripherals *
global _irq12 ; PS2 mouse interrupt
global _irq13 ; FPU interrupt
global _irq14 ; primary ATA hard disk
global _irq15 ; secondary ATA hard disk


section .text

_irq0:
   pusha
   push   ds
   push   gs
   push   fs
   push   es
   mov    bx, __KERNEL_DS
   mov    ds, bx
   mov    gs, bx
   mov    es, bx
   mov    fs, bx
   call   handle_irq0
   pop    es
   pop    fs
   pop    gs
   pop    ds
   mov    al, __PIC8259_EOI_CMD ; signal EOI to master PIC
   out    __PIC8259_MASTER_CMD_PORT, al 
   popa
   iret

_irq1:
   pusha
   push   ds
   push   gs
   push   fs
   push   es
   mov    bx, __KERNEL_DS
   mov    ds, bx
   mov    gs, bx
   mov    es, bx
   mov    fs, bx
   call   handle_irq1
   pop    es
   pop    fs
   pop    gs
   pop    ds
   mov    al, __PIC8259_EOI_CMD
   out    __PIC8259_MASTER_CMD_PORT, al
   popa
   iret

_irq2:
   ; cascade interrupt used internally by master/slave PICs
   ; should never be raised so we just use hlt here
   hlt

_irq3:
   pusha
   push   ds
   push   gs
   push   fs
   push   es
   mov    bx, __KERNEL_DS
   mov    ds, bx
   mov    gs, bx
   mov    es, bx
   mov    fs, bx
   call   handle_irq3
   pop    es
   pop    fs
   pop    gs
   pop    ds
   mov    al, __PIC8259_EOI_CMD
   out    __PIC8259_MASTER_CMD_PORT, al
   popa
   iret

_irq4:
   pusha
   push   ds
   push   gs
   push   fs
   push   es
   mov    bx, __KERNEL_DS
   mov    ds, bx
   mov    gs, bx
   mov    es, bx
   mov    fs, bx
   call   handle_irq4
   pop    es
   pop    fs
   pop    gs
   pop    ds
   mov    al, __PIC8259_EOI_CMD
   out    __PIC8259_MASTER_CMD_PORT, al
   popa
   iret

_irq5:
   pusha
   push   ds
   push   gs
   push   fs
   push   es
   mov    bx, __KERNEL_DS
   mov    ds, bx
   mov    gs, bx
   mov    es, bx
   mov    fs, bx
   call   handle_irq5
   pop    es
   pop    fs
   pop    gs
   pop    ds
   mov    al, __PIC8259_EOI_CMD
   out    __PIC8259_MASTER_CMD_PORT, al
   popa
   iret

_irq6:
   pusha
   push   ds
   push   gs
   push   fs
   push   es
   mov    bx, __KERNEL_DS
   mov    ds, bx
   mov    gs, bx
   mov    es, bx
   mov    fs, bx
   call   handle_irq6
   pop    es
   pop    fs
   pop    gs
   pop    ds
   mov    al, __PIC8259_EOI_CMD
   out    __PIC8259_MASTER_CMD_PORT, al
   popa
   iret

_irq7:
   pusha
   ; handle master PIC's spurious interrupt if exist
   mov    al, __PIC8259_ISR_READ_CMD
   out    __PIC8259_MASTER_CMD_PORT, al
   in     al, __PIC8259_MASTER_CMD_PORT
   and    al, 0x80
   jz     _pic8259_master_spurious_intrpt
   push   ds
   push   gs
   push   fs
   push   es
   mov    bx, __KERNEL_DS
   mov    ds, bx
   mov    gs, bx
   mov    es, bx
   mov    fs, bx
   call   handle_irq7
   pop    es
   pop    fs
   pop    gs
   pop    ds
   mov    al, __PIC8259_EOI_CMD
   out    __PIC8259_MASTER_CMD_PORT, al
_pic8259_master_spurious_intrpt:
   popa
   iret

_irq8:
   pusha
   push   ds
   push   gs
   push   fs
   push   es
   mov    bx, __KERNEL_DS
   mov    ds, bx
   mov    gs, bx
   mov    es, bx
   mov    fs, bx
   call   handle_irq8
   pop    es
   pop    fs
   pop    gs
   pop    ds
   ; signal EOI to master && slave PICs
   mov    al, __PIC8259_EOI_CMD
   out    __PIC8259_SLAVE_CMD_PORT, al
   out    __PIC8259_MASTER_CMD_PORT, al
   popa
   iret

_irq9:
   pusha
   push   ds
   push   gs
   push   fs
   push   es
   mov    bx, __KERNEL_DS
   mov    ds, bx
   mov    gs, bx
   mov    es, bx
   mov    fs, bx
   call   handle_irq9
   pop    es
   pop    fs
   pop    gs
   pop    ds
   mov    al, __PIC8259_EOI_CMD
   out    __PIC8259_SLAVE_CMD_PORT, al
   out    __PIC8259_MASTER_CMD_PORT, al
   popa
   iret

_irq10:
   pusha
   push   ds
   push   gs
   push   fs
   push   es
   mov    bx, __KERNEL_DS
   mov    ds, bx
   mov    gs, bx
   mov    es, bx
   mov    fs, bx
   call   handle_irq10
   pop    es
   pop    fs
   pop    gs
   pop    ds
   mov    al, __PIC8259_EOI_CMD
   out    __PIC8259_SLAVE_CMD_PORT, al
   out    __PIC8259_MASTER_CMD_PORT, al
   popa
   iret

_irq11:
   pusha
   push   ds
   push   gs
   push   fs
   push   es
   mov    bx, __KERNEL_DS
   mov    ds, bx
   mov    gs, bx
   mov    es, bx
   mov    fs, bx
   call   handle_irq11
   pop    es
   pop    fs
   pop    gs
   pop    ds
   mov    al, __PIC8259_EOI_CMD
   out    __PIC8259_SLAVE_CMD_PORT, al
   out    __PIC8259_MASTER_CMD_PORT, al
   popa
   iret

_irq12:
   pusha
   push   ds
   push   gs
   push   fs
   push   es
   mov    bx, __KERNEL_DS
   mov    ds, bx
   mov    gs, bx
   mov    es, bx
   mov    fs, bx
   call   handle_irq12
   pop    es
   pop    fs
   pop    gs
   pop    ds
   mov    al, __PIC8259_EOI_CMD
   out    __PIC8259_SLAVE_CMD_PORT, al
   out    __PIC8259_MASTER_CMD_PORT, al
   popa
   iret

_irq13:
   pusha
   push   ds
   push   gs
   push   fs
   push   es
   mov    bx, __KERNEL_DS
   mov    ds, bx
   mov    gs, bx
   mov    es, bx
   mov    fs, bx
   call   handle_irq13
   pop    es
   pop    fs
   pop    gs
   pop    ds
   mov    al, __PIC8259_EOI_CMD
   out    __PIC8259_SLAVE_CMD_PORT, al
   out    __PIC8259_MASTER_CMD_PORT, al
   popa
   iret

_irq14:
   pusha
   push   es
   push   gs
   push   fs
   push   es
   mov    bx, __KERNEL_DS
   mov    ds, bx
   mov    gs, bx
   mov    es, bx
   mov    fs, bx
   call   handle_irq14
   pop    es
   pop    fs
   pop    gs
   pop    ds
   mov    al, __PIC8259_EOI_CMD
   out    __PIC8259_SLAVE_CMD_PORT, al
   out    __PIC8259_MASTER_CMD_PORT, al
   popa
   iret

_irq15:
   pusha
   ; handle slave PIC's spurious interrupt if occurred
   mov    al, __PIC8259_ISR_READ_CMD
   out    __PIC8259_SLAVE_CMD_PORT, al
   in     al, __PIC8259_SLAVE_CMD_PORT
   and    al, 0x80
   mov    al, __PIC8259_EOI_CMD
   jz     _pic8259_slave_spurious_intrpt
   push   ds
   push   gs
   push   fs
   push   es
   mov    bx, __KERNEL_DS
   mov    ds, bx
   mov    gs, bx
   mov    es, bx
   mov    fs, bx
   call   handle_irq15
   pop    es
   pop    fs
   pop    gs
   pop    ds
   mov    al, __PIC8259_EOI_CMD
   out    __PIC8259_SLAVE_CMD_PORT, al
_pic8259_slave_spurious_intrpt:
   out    __PIC8259_MASTER_CMD_PORT, al
   popa
   iret

; cpu divide by zero exception
_cpu_div_zero:
    cli
    pusha
    push  ds
    push  gs
    push  fs
    push  es
    mov   bx, __KERNEL_DS
    mov   ds, bx
    mov   gs, bx
    mov   es, bx
    mov   fs, bx
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
    push  ds
    push  gs
    push  fs
    push  es
    mov   bx, __KERNEL_DS
    mov   ds, bx
    mov   gs, bx
    mov   es, bx
    mov   fs, bx
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
    push  ds
    push  gs
    push  fs
    push  es
    mov   bx, __KERNEL_DS
    mov   ds, bx
    mov   gs, bx
    mov   es, bx
    mov   fs, bx
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
    push  ds
    push  gs
    push  fs
    push  es
    mov   bx, __KERNEL_DS
    mov   ds, bx
    mov   gs, bx
    mov   es, bx
    mov   fs, bx
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
    push  ds
    push  gs
    push  fs
    push  es
    mov   bx, __KERNEL_DS
    mov   ds, bx
    mov   gs, bx
    mov   es, bx
    mov   fs, bx
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
    push  ds
    push  gs
    push  fs
    push  es
    mov   bx, __KERNEL_DS
    mov   ds, bx
    mov   gs, bx
    mov   es, bx
    mov   fs, bx
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
    push  ds
    push  gs
    push  fs
    push  es
    mov   bx, __KERNEL_DS
    mov   ds, bx
    mov   gs, bx
    mov   es, bx
    mov   fs, bx
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
    push  ds
    push  gs
    push  fs
    push  es
    mov   bx, __KERNEL_DS
    mov   ds, bx
    mov   gs, bx
    mov   es, bx
    mov   fs, bx
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
    push  ds
    push  gs
    push  fs
    push  es
    mov   bx, __KERNEL_DS
    mov   ds, bx
    mov   gs, bx
    mov   es, bx
    mov   fs, bx
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
    push  ds
    push  gs
    push  fs
    push  es
    mov   bx, __KERNEL_DS
    mov   ds, bx
    mov   gs, bx
    mov   es, bx
    mov   fs, bx
    call  handle_cpu_general_protection_fault
    pop   es
    pop   fs
    pop   gs
    pop   ds
    popa
    add   esp, 0x4
    sti
    iret
