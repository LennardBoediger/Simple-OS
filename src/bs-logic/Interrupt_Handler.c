
//
// Created by Lennard on 02.11.18.
//
#include "../include/Interrupt_Handler.h"
#include "../include/kprintf.h"

void reset() {

    kprintf("RESET");
    kprintf("r13_system: %i \n\r\n\r");
}

void undef(uint32_t stackadress) {
    int i = 0;
    for (i; i<=15; i++) {
        kprintf("R%u: %i\n\r", i, *(int*) (stackadress+i*4));
    }
/*    kprintf("Stackadress: %i\n\r\n\r", stackadress);
    kprintf("r0_system: %i \n\r\n\r", *(int*) (stackadress));
    kprintf("r1_system: %i \n\r\n\r", *(int*) (stackadress+4));
    kprintf("r2_system: %i \n\r\n\r", stackadress+13*4);
    kprintf("r3_system: %i \n\r\n\r", stackadress+12*4);
    kprintf("r4_system: %i \n\r\n\r", stackadress+11*4);
    kprintf("r5_system: %i \n\r\n\r", stackadress+10*4);
    kprintf("r6_system: %i \n\r\n\r", stackadress+9*4);
    kprintf("r7_system: %i \n\r\n\r", stackadress+8*4);
    kprintf("r8_system: %i \n\r\n\r", stackadress+7*4);
    kprintf("r9_system: %i \n\r\n\r", stackadress+6*4);
    kprintf("r10_system: %i \n\r\n\r", stackadress+5*4);
    kprintf("r11_system: %i \n\r\n\r", stackadress+4*4);
    kprintf("r12_system: %i \n\r\n\r", stackadress+3*4);
    kprintf("r13_system: %i \n\r\n\r", stackadress+2*4);
    kprintf("r14_system: %i \n\r\n\r", stackadress+1*4);
    kprintf("r15_system: %i \n\r\n\r", stackadress+0*4);*/
    }