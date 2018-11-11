//
// Created by Lennard on 02.11.18.
//
#ifndef BS_PRAK_INTERRUPT_HANDLER_H
#define BS_PRAK_INTERRUPT_HANDLER_H
#include <stdint.h>

void print_interrupt(uint32_t stackadress, uint32_t cpsr, uint32_t spsr, char* interrupt_name);

void watchdog();

void reset(uint32_t stackadress, uint32_t cpsr, uint32_t spsr);

void undef(uint32_t stackadress, uint32_t cpsr, uint32_t spsr);

void swi(uint32_t stackadress, uint32_t cpsr, uint32_t spsr);

void prefab(uint32_t stackadress, uint32_t cpsr, uint32_t spsr);

void dataab(uint32_t stackadress, uint32_t cpsr, uint32_t spsr);

void irq(uint32_t stackadress, uint32_t cpsr, uint32_t spsr);

void fiq(uint32_t stackadress, uint32_t cpsr, uint32_t spsr);

struct timer {
    uint32_t LOAD;
    uint32_t VALUE;
    uint32_t CONTROL;
    uint32_t IRQ_CLEAR_ACK;
    uint32_t unused[3];
    uint32_t PREDIVIDER;
};

struct en_basic_irq {
    uint32_t IRQ_BASIC_PENDING;
    uint32_t unused[5];
    uint32_t EN_BASIC_IRQS;
};

#endif //BS_PRAK_INTERRUPT_HANDLER_H
