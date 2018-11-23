
#ifndef BS_PRAK_INTERRUPT_REGS_DRIVER_H
#define BS_PRAK_INTERRUPT_REGS_DRIVER_H
#include <stdint.h>

static volatile
struct timer* const timer_reg;

//static volatile
struct timer {
    uint32_t LOAD;
    uint32_t VALUE;
    uint32_t CONTROL;
    uint32_t IRQ_CLEAR_ACK;
    uint32_t unused[3];
    uint32_t PREDIVIDER;
};

static volatile
struct arm_interrupt* const arm_interrupt_reg;

//static volatile
struct arm_interrupt {
    uint32_t IRQ_BASIC_PENDING;
    uint32_t IRQ_PENDING_1;
    uint32_t IRQ_PENDING_2;
    uint32_t FIQ_CONTROL;
    uint32_t EN_IRQ_1;
    uint32_t EN_IRQ_2;
    uint32_t EN_BASIC_IRQS;
    uint32_t DIS_IRQ_1;
    uint32_t DIS_IRQ_2;
    uint32_t DIS_BASIC_IRQS;
};

void initialize_timer();
void clear_timer();
void enable_IRQ_interrupts();
uint32_t recognize_irq_interrupt(uint32_t irq_stackadress, uint32_t spsr);
void force_interrupts(char c);

#endif //BS_PRAK_INTERRUPT_REGS_DRIVER_H
