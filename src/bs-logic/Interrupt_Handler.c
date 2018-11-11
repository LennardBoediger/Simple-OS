#include <math.h>
#include "../include/Interrupt_Handler.h"
#include "../include/kprintf.h"

/** ab hier defines für timer*/
#define TIMER_BASE (0x7E00B000 - 0x3F000000) //timerbaseadress minus MMU-offset
#define IRQ_FIQ_REG_OFFSET 0x200
#define TIMER_EN_SHIFT 7
#define TIMER_INTERRUPT_EN_SHIFT 5
#define TIMER_PRESCALE_MSB_SHIFT 3
#define TIMER_PRESCALE_LSB_SHIFT 2
#define TIMER_32BIT_COUNTER 1
#define ARM_TIMER 0
#define PREDIVIDER_VALUE 42 //1023 /* 250MHz -> Prescale -> Predivider -> 954,6 Hz*/
#define TIMER_VALUE 10//954


static volatile
struct timer* const watchdog_reg = (struct timer*) TIMER_BASE;

/* Setzt/-erneut watchdog-timer auf 1 sec mit Interrupt */
void watchdog() {
    watchdog_reg->LOAD = TIMER_VALUE;
    watchdog_reg->CONTROL |= ((1 << TIMER_INTERRUPT_EN_SHIFT) | (1 << TIMER_PRESCALE_MSB_SHIFT) | (1 << TIMER_32BIT_COUNTER));
    watchdog_reg->CONTROL &= ~(1 << TIMER_PRESCALE_LSB_SHIFT);
    watchdog_reg->PREDIVIDER = PREDIVIDER_VALUE;
    watchdog_reg->CONTROL = (1 << TIMER_EN_SHIFT); /* Timer starten */
};

static volatile
struct en_basic_irq* const en_basic_irq_reg = (struct en_basic_irq*) (TIMER_BASE + IRQ_FIQ_REG_OFFSET);

/* aktiviert Timer- und UART-Interrupts*/
void enable_ext_interrupts () {
    en_basic_irq_reg->EN_BASIC_IRQS |= (1 << ARM_TIMER);
}

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

void swi(uint32_t stackadress, uint32_t cpsr, uint32_t spsr) {
    print_interrupt(stackadress, cpsr, spsr, "SOFTWARE INTERRUPT");
}


void prefab(uint32_t stackadress, uint32_t cpsr, uint32_t spsr) {
    print_interrupt(stackadress, cpsr, spsr, "PREFETCH ABORT");
}

void dataab(uint32_t stackadress, uint32_t cpsr, uint32_t spsr) {
    print_interrupt(stackadress, cpsr, spsr, "DATA ABORT");
}

void irq(uint32_t stackadress, uint32_t cpsr, uint32_t spsr) {
    print_interrupt(stackadress, cpsr, spsr, "TIMER INTERRUPT");
    watchdog();
}

void fiq(uint32_t stackadress, uint32_t cpsr, uint32_t spsr) {
    print_interrupt(stackadress, cpsr, spsr, "DATA ABORT");
}