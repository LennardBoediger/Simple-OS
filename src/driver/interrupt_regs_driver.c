//
// Created by pablo on 12.11.18.
//
#include "../include/interrupt_regs_driver.h"
#include "../include/kprintf.h"
#include "../include/uart_driver.h"

#define IRQ_FIQ_REG_OFFSET (0x7E00B000 - 0x3F000000 + 0x200) //TODO: change name 0x200
#define TIMER_BASE (0x7E00B000 - 0x3F000000 + 0x400) //timerbaseadress minus MMU-offset
#define PREDIVIDER_VALUE 249 /* +1 = 250*/
#define TIMER_VALUE 5000000//5000000 //5 sek
#define TIMER_EN_SHIFT 7
#define TIMER_INTERRUPT_EN_SHIFT 5
#define TIMER_32BIT_COUNTER 1
#define IRQ_TIMER_SHIFT 0
#define IRQ_UART_SHIFT 25 /*bei irq_pending_2 entspricht das #57*/


static volatile
struct timer* const timer_reg = (struct timer*) TIMER_BASE;

static volatile
struct arm_interrupt* const arm_interrupt_reg = (struct arm_interrupt*) (IRQ_FIQ_REG_OFFSET);


void initialize_timer() {
    timer_reg->PREDIVIDER = PREDIVIDER_VALUE;
    timer_reg->CONTROL = 0x0;
    timer_reg->CONTROL |= ((1 << TIMER_INTERRUPT_EN_SHIFT) | (1 << TIMER_32BIT_COUNTER));
    timer_reg->CONTROL |= (1 << TIMER_EN_SHIFT); /* Timer starten */
    timer_reg->LOAD = TIMER_VALUE;
    kprintf("INITIALIZE TIMER\n\r Controlreg: %x \n\r", timer_reg->CONTROL);
    kprintf("Predivider: %x \n\r", timer_reg->PREDIVIDER);
    kprintf("Timervalue: %i\n\r",timer_reg->VALUE);
};

void clear_timer() {
    timer_reg->IRQ_CLEAR_ACK = 0x1; // TODO:has to happen
  /*  kprintf("\n\r-----------clear_timer-------------\n\r");
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
    kprintf("\n\r%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n\r");*/
}

/* aktiviert Timer- und UART-Interrupts*/
void enable_ext_interrupts () {
    arm_interrupt_reg->EN_BASIC_IRQS = (1 << IRQ_TIMER_SHIFT);
    arm_interrupt_reg->EN_IRQ_2 = (1 << IRQ_UART_SHIFT);
    en_uart_interrupt();
    kprintf("\n\rEnable_ext_Interrupts\n\r");
}

void recognize_irq_interrupt() {
    if (((arm_interrupt_reg->IRQ_BASIC_PENDING & (1 << IRQ_TIMER_SHIFT))>>IRQ_TIMER_SHIFT) == 1) {
        kprintf("\n\r!\n\r");
        clear_timer();
    }
    if (((arm_interrupt_reg->IRQ_PENDING_2 & (1 << IRQ_UART_SHIFT))>>IRQ_UART_SHIFT) == 1) {
        kprintf("UART TASTENDRUCK!!!");
    }
}