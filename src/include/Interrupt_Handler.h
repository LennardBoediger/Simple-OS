//
// Created by Lennard on 02.11.18.
//
#ifndef BS_PRAK_INTERRUPT_HANDLER_H
#define BS_PRAK_INTERRUPT_HANDLER_H
#include <stdint.h>

void reset(uint32_t stackadress, uint32_t cpsr, uint32_t spsr);

void undef(uint32_t stackadress, uint32_t cpsr, uint32_t spsr);

struct timer {
    uint32_t LOAD;
    uint32_t VALUE;
    uint32_t CONTROL;
    uint32_t IRQ_CLEAR/ACK;
    uint32_t unused[3];
    uint32_t PREDIVIDER;

};

#endif //BS_PRAK_INTERRUPT_HANDLER_H
