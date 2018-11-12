#include <math.h>
#include "../include/Interrupt_Handler.h"
#include "../include/kprintf.h"

/** ab hier defines für timer*/
#define TIMER_BASE (0x7E00B000 - 0x3F000000 + 0x400) //timerbaseadress minus MMU-offset
#define IRQ_FIQ_REG_OFFSET 0x200
#define TIMER_EN_SHIFT 7
#define TIMER_INTERRUPT_EN_SHIFT 5
#define TIMER_PRESCALE_MSB_SHIFT 3
#define TIMER_PRESCALE_LSB_SHIFT 2
#define TIMER_32BIT_COUNTER 1
#define IRQ_TIMER_SHIFT 0
#define IRQ_UART_SHIFT 52
#define PREDIVIDER_VALUE 249 /* +1 = 250*/
#define TIMER_VALUE 5000


static volatile
struct timer* const timer_reg = (struct timer*) TIMER_BASE;

/* Setzt/-erneut initialize_timer-timer auf 1 sec mit Interrupt */
void initialize_timer() {
    timer_reg->PREDIVIDER = PREDIVIDER_VALUE;
    timer_reg->CONTROL = 0x0;
    timer_reg->CONTROL |= ((1 << TIMER_INTERRUPT_EN_SHIFT) | /*(1 << TIMER_PRESCALE_MSB_SHIFT) |*/ (1 << TIMER_32BIT_COUNTER));
    timer_reg->CONTROL |= (1 << TIMER_EN_SHIFT); /* Timer starten */
    timer_reg->LOAD = TIMER_VALUE;
    kprintf("Controlreg: %x \n\r", timer_reg->CONTROL);
    kprintf("Predivider: %x \n\r", timer_reg->PREDIVIDER);
    kprintf("Timervalue: %i\n\r",timer_reg->VALUE);
    kprintf("Timervalue: %i\n\r",timer_reg->VALUE);
    kprintf("Timervalue: %i\n\r",timer_reg->VALUE);
    kprintf("Timervalue: %i\n\r",timer_reg->VALUE);
    kprintf("Timervalue: %i\n\r",timer_reg->VALUE);
    kprintf("Timervalue: %i\n\r",timer_reg->VALUE);
    kprintf("Timervalue: %i\n\r",timer_reg->VALUE);
    kprintf("Timervalue: %i\n\r",timer_reg->VALUE);

};

void clear_timer() {
    timer_reg->IRQ_CLEAR_ACK = 0x1;
}

static volatile
struct arm_interrupt* const arm_interrupt_reg = (struct arm_interrupt*) (TIMER_BASE + IRQ_FIQ_REG_OFFSET);

/* aktiviert Timer- und UART-Interrupts*/
void enable_ext_interrupts () {
    arm_interrupt_reg->EN_BASIC_IRQS = (1 << IRQ_TIMER_SHIFT);
    arm_interrupt_reg->EN_IRQ_2 = (1 << IRQ_UART_SHIFT);
}

void recognize_irq_interrupt() {
    if ((arm_interrupt_reg->IRQ_BASIC_PENDING & (1 << IRQ_TIMER_SHIFT)) == 1) {
        kprintf("!\n\r");
        clear_timer();
    }
    if ((arm_interrupt_reg->IRQ_PENDING_2 & (1 << IRQ_UART_SHIFT)) == 1) {
        kprintf("UART TASTENDRUCK!!!");
    }
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
    kprintf("TIME INTERRUPT");
//    print_interrupt(stackadress, cpsr, spsr, "TIMER INTERRUPT");
    recognize_irq_interrupt();
//    initialize_timer();
}

void fiq(uint32_t stackadress, uint32_t cpsr, uint32_t spsr) {
    print_interrupt(stackadress, cpsr, spsr, "DATA ABORT");
}