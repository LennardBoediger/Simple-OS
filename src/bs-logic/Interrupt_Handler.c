
//
// Created by Lennard on 02.11.18.
//
#include "../include/Interrupt_Handler.h"
#include "../include/kprintf.h"

void reset() {
    kprintf("r0_system: %i \n\r\n\r");
    kprintf("r13_system: %i \n\r\n\r");
}

void undef() {
    kprintf("r0_system: %i \n\r\n\r");
    kprintf("r13_system: %i \n\r\n\r");
}