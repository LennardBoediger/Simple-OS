
//
// Created by Lennard on 02.11.18.
//
#include "../include/Interrupt_Handler.h"
#include "../include/kprintf.h"
void print_interrupt(unit32_t stackadress, char* intrrupt_name){
    int i;
    for (i = 0; i < 46; ++i) {
        kprintf("#");
    }
    kprintf("%s:",intrrupt_name);
    for (i = 0; i <= 7; i++) {
        kprintf("R%u: %i\t", i, *(int*) (stackadress+i*4));
        kprintf("R%u: %i\n\r", i+7, *(int*) (stackadress+(i+7)*4));
    }
    kprintf(">>> Registerschnappschuss (aktueller Modus) <<<\n\r")

}
void reset() {

    kprintf("RESET");
    kprintf("r13_system: %i \n\r\n\r");
}

void undef(uint32_t stackadress) {
    print_interrupt(stackadress, "UNDEFINED")
}