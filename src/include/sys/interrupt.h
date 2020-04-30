#ifndef __INTERRUPT_H
#define __INTERRUPT_H

extern void handle_cpu_div_zero (void);
extern void handle_cpu_nmskable_intrpt (void);
extern void handle_cpu_brkpoint (void);
extern void handle_cpu_overflow (void);
extern void handle_cpu_invalid_opcode (void);
extern void handle_cpu_double_fault (void);
extern void handle_cpu_invalid_tss (void);
extern void handle_cpu_segment_not_prsnt (void);
extern void handle_cpu_stack_segment_fault (void);
extern void handle_cpu_general_protection_fault (void);
extern void default_interrupt_handler (void);

#endif /* __INTERRUPT_H */
