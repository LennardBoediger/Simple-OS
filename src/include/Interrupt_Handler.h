//
// Created by Lennard on 02.11.18.
//
#ifndef BS_PRAK_INTERRUPT_HANDLER_H
#define BS_PRAK_INTERRUPT_HANDLER_H
#include <stdint.h>

void print_interrupt(uint32_t stackadress, uint32_t cpsr, uint32_t spsr, char* interrupt_name, int32_t pc_offset);


void print_timerval(uint32_t cpsr);

void reset(uint32_t stackadress, uint32_t cpsr, uint32_t spsr);
void undef(uint32_t stackadress, uint32_t cpsr, uint32_t spsr);
void swi(uint32_t stackadress, uint32_t cpsr, uint32_t spsr);
void prefab(uint32_t stackadress, uint32_t cpsr, uint32_t spsr);
void dataab(uint32_t stackadress, uint32_t cpsr, uint32_t spsr);
void irq(uint32_t stackadress, uint32_t cpsr, uint32_t spsr);
void fiq(uint32_t stackadress, uint32_t cpsr, uint32_t spsr);


#endif //BS_PRAK_INTERRUPT_HANDLER_H
