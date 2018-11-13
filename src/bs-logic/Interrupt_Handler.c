#include "../include/kprintf.h"
#include "../include/interrupt_regs_driver.h"

/** ab hier defines für timer*/

#define TIMER_PRESCALE_MSB_SHIFT 3
#define TIMER_PRESCALE_LSB_SHIFT 2
#define WNR_BIT_SHIFT 11


void print_timerval(uint32_t cpsr) {
    kprintf("Timervalue: %i\n\r",timer_reg->VALUE);
    kprintf("CPSR: %x", cpsr);
}
void print_data_abort_reason(){
    uint32_t dfar = read_dfar();
    uint32_t dfsr = read_dfsr();
    if((dfsr & (1 << WNR_BIT_SHIFT)) >> WNR_BIT_SHIFT){
        kprintf("Zugriff: schreibend auf Adresse %x\n", dfar);
    } else{
        kprintf("Zugriff: lesend auf Adresse %x\n", dfar);
    }
}
void print_interrupt(uint32_t stackadress, uint32_t cpsr, uint32_t spsr, char* interrupt_name, int32_t pc_offset){
    kprintf("\n\r");
    int i;
    for (i = 0; i < 47; ++i) {
        kprintf("#");
    }
    //PC wird bei Ausnahme nach LR geschrieben + wegen pipline muss offset bechatet werden
    kprintf("\n\r%s an der Adresse %x\n\r",interrupt_name, *(int*) (stackadress+14*4) + pc_offset);

    //DFSR auslesen und Ausgabe mit switch-case verschönern

    kprintf(">>> Registerschnappschuss (aktueller Modus) <<<\n\r");
    for (i = 0; i <= 7; i++) {
        kprintf("R%u: %x\t", i, *(int*) (stackadress+i*4));
        kprintf("R%u: %x\n\r", i+8, *(int*) (stackadress+(i+8)*4));
    }

    kprintf(">>> Aktuelle Statusregister (SPSR im aktuellen Modus) <<<\n\r");
    kprintf("CPSR: %x\n\r", cpsr);
    kprintf("SPSR: %x\n\r", spsr);
    for (i = 0; i < 47; ++i) {
        kprintf("#");
    }
    kprintf("\n\r");
}


void reset(uint32_t stackadress, uint32_t cpsr, uint32_t spsr) {
    print_interrupt(stackadress, cpsr, spsr, "RESET", 0);
}

void undef(uint32_t stackadress, uint32_t cpsr, uint32_t spsr) {
    print_interrupt(stackadress, cpsr, spsr, "UNDEFINED", -4);
}

void swi(uint32_t stackadress, uint32_t cpsr, uint32_t spsr) {
    print_interrupt(stackadress, cpsr, spsr, "SOFTWARE INTERRUPT", -4);
}


void prefab(uint32_t stackadress, uint32_t cpsr, uint32_t spsr) {
    print_interrupt(stackadress, cpsr, spsr, "PREFETCH ABORT", -4);
}

void dataab(uint32_t stackadress, uint32_t cpsr, uint32_t spsr) {
    print_interrupt(stackadress, cpsr, spsr, "DATA ABORT", -8);
    print_data_abort_reason();
}

void irq(uint32_t stackadress, uint32_t cpsr, uint32_t spsr) {
    print_interrupt(stackadress, cpsr, spsr, "TIMER INTERRUPT", -8);
    recognize_irq_interrupt();
}

void fiq(uint32_t stackadress, uint32_t cpsr, uint32_t spsr) {
    print_interrupt(stackadress, cpsr, spsr, "DATA ABORT", -8);
}