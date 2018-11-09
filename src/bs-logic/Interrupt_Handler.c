#include "../include/Interrupt_Handler.h"
#include "../include/kprintf.h"
#define TIMER_BASE 0x7E00B000 //TODO: kein Range??


static volatile
struct timer* const watchdog_reg = (struct timer*) TIMER_BASE;


void print_interrupt(uint32_t stackadress, uint32_t cpsr, uint32_t spsr, char* interrupt_name){
    kprintf("\n\r");
    int i;
    for (i = 0; i < 47; ++i) {
        kprintf("#");
    }
    kprintf("\n\r%s\n\r",interrupt_name);

    //DFSR auslesen und Ausgabe mit switch-case verschönern

    kprintf(">>> Registerschnappschuss (aktueller Modus) <<<\n\r");
    for (i = 0; i <= 7; i++) {
        kprintf("R%u: %x\t", i, *(int*) (stackadress+i*4));
        kprintf("R%u: %x\n\r", i+8, *(int*) (stackadress+(i+8)*4));
    }

    kprintf(">>> Aktuelle Statusregister (SPSR im aktuellen Modus) <<<\n\r");
    kprintf("CPSR: %u\n\r", cpsr);
    kprintf("SPSR: %u\n\r", spsr);
    for (i = 0; i < 47; ++i) {
        kprintf("#");
    }
    kprintf("\n\r");
}


void reset(uint32_t stackadress, uint32_t cpsr, uint32_t spsr) {
    print_interrupt(stackadress, cpsr, spsr, "RESET");
}

void undef(uint32_t stackadress, uint32_t cpsr, uint32_t spsr) {
    print_interrupt(stackadress, cpsr, spsr, "UNDEFINED");
}