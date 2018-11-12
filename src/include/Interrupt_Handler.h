//
// Created by Lennard on 02.11.18.
//
#ifndef BS_PRAK_INTERRUPT_HANDLER_H
#define BS_PRAK_INTERRUPT_HANDLER_H
#include <stdint.h>

void print_interrupt(uint32_t stackadress, uint32_t cpsr, uint32_t spsr, char* interrupt_name);

static volatile
struct timer* const timer_reg;

void initialize_timer();

void print_timerval(uint32_t cpsr);

void reset(uint32_t stackadress, uint32_t cpsr, uint32_t spsr);

void undef(uint32_t stackadress, uint32_t cpsr, uint32_t spsr);

void swi(uint32_t stackadress, uint32_t cpsr, uint32_t spsr);

void prefab(uint32_t stackadress, uint32_t cpsr, uint32_t spsr);

void dataab(uint32_t stackadress, uint32_t cpsr, uint32_t spsr);

void irq(uint32_t stackadress, uint32_t cpsr, uint32_t spsr);

void fiq(uint32_t stackadress, uint32_t cpsr, uint32_t spsr);

static volatile struct timer {
    uint32_t LOAD;
    uint32_t VALUE;
    uint32_t CONTROL;
    uint32_t IRQ_CLEAR_ACK;
    uint32_t unused[3];
    uint32_t PREDIVIDER;
};

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

#endif //BS_PRAK_INTERRUPT_HANDLER_H
