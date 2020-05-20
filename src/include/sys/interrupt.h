#ifndef __INTERRUPT_H
#define __INTERRUPT_H

/*
 * CPU's exceptions' handler's functions prototypes
*/
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

/*
 * IRQs' handler's functions prototypes
*/
extern void handle_irq0(void);
extern void handle_irq1(void);
extern void handle_irq3(void);
extern void handle_irq4(void);
extern void handle_irq5(void);
extern void handle_irq6(void);
extern void handle_irq7(void);
extern void handle_irq8(void);
extern void handle_irq9(void);
extern void handle_irq10(void);
extern void handle_irq12(void);
extern void handle_irq13(void);
extern void handle_irq14(void);
extern void handle_irq15(void);

#endif /* __INTERRUPT_H */
