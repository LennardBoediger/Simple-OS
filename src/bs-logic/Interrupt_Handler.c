#include "../include/kprintf.h"
#include "../include/interrupt_regs_driver.h"
#include "../include/Interrupt_printer.h"
#include "../include/mode_reg_helper.h"
#include "../include/dataab_helper.h"
#include "../include/threads_handler.h"
#include "../include/init_thread.h"


uint8_t debug_irq = 1;  //Global RINT IRQ


void reset(uint32_t stackadress, uint32_t cpsr, uint32_t spsr) {
    print_interrupt(stackadress, cpsr, spsr, "RESET", 0, 0);
}

void undef(uint32_t stackadress, uint32_t cpsr, uint32_t spsr) {
    print_interrupt(stackadress, cpsr, spsr, "UNDEFINED", -4, 0);
    //Nur Usermode ist % 16 == 0
    if ((spsr % 16) == 0){
        get_tcb(get_running_thread())->zustand = BEENDET;
    }
}

void swi(uint32_t stackadress, uint32_t cpsr, uint32_t spsr) {
    print_interrupt(stackadress, cpsr, spsr, "SOFTWARE INTERRUPT", -4, 0);
    if ((spsr % 16) == 0){
        get_tcb(get_running_thread())->zustand = BEENDET;
    }
}


void prefab(uint32_t stackadress, uint32_t cpsr, uint32_t spsr) {
    print_interrupt(stackadress, cpsr, spsr, "PREFETCH ABORT", -4, 0);
}

void dataab(uint32_t stackadress, uint32_t cpsr, uint32_t spsr) {
    print_interrupt(stackadress, cpsr, spsr, "DATA ABORT", -8, 1);
    if ((spsr % 16) == 0){
        get_tcb(get_running_thread())->zustand = BEENDET;
    }
}

uint32_t irq(uint32_t irq_stackadress, uint32_t cpsr, uint32_t spsr) {
    if (debug_irq == 1){
        print_interrupt(irq_stackadress, cpsr, spsr, "IRQ INTERRUPT", -8, 0);
    }
    return recognize_irq_interrupt(irq_stackadress, spsr);
}

void fiq(uint32_t stackadress, uint32_t cpsr, uint32_t spsr) {
    print_interrupt(stackadress, cpsr, spsr, "DATA ABORT", -8, 0);
}