
#ifndef BS_PRAK_INTERRUPT_HANDLER_H
#define BS_PRAK_INTERRUPT_HANDLER_H
#include <stdint.h>

#define MODE_MASK 32
#define USER_MODE 16

extern uint8_t debug_irq; //Global RINT IRQ

void reset(uint32_t stackadress, uint32_t cpsr, uint32_t spsr);
uint32_t undef(uint32_t stackadress, uint32_t cpsr, uint32_t spsr);
uint32_t swi(uint32_t stackadress, uint32_t cpsr, uint32_t spsr);
uint32_t prefab(uint32_t stackadress, uint32_t cpsr, uint32_t spsr);
uint32_t dataab(uint32_t stackadress, uint32_t cpsr, uint32_t spsr);
uint32_t irq(uint32_t stackadress, uint32_t cpsr, uint32_t spsr);
void fiq(uint32_t stackadress, uint32_t cpsr, uint32_t spsr);


#endif //BS_PRAK_INTERRUPT_HANDLER_H
