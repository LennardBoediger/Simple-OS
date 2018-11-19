#include "../include/uart_driver.h"
#include "../include/kprintf.h"
#include "../include/Interrupt_Handler.h"
#include "../include/dataab_helper.h"
#include "../include/regcheck.h"
#include "../include/interrupt_regs_driver.h"
#include "../include/threads_handler.h"
#include "../include/init_thread.h"

#define IRQ_FIQ_REG_OFFSET (0x7E00B000 - 0x3F000000 + 0x200)
#define TIMER_BASE (0x7E00B000 - 0x3F000000 + 0x400) //timerbaseadress minus MMU-offset
#define PREDIVIDER_VALUE 249 /* +1 = 250*/
#define INTERRUPT_TIMER 2000000  /* HIER HAEUFIGKEIT DER INTERRUPTS EINSTELLBAR */
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
    timer_reg->LOAD = INTERRUPT_TIMER;
};

void clear_timer() {
    timer_reg->IRQ_CLEAR_ACK = 0x1;
}


/* erzeugt kurze Pausen zwischen den Buchstaben. 52147 = magic number */
static void wait() {
    int i;
    for (i = 0; i < 52147; i++) {
        asm("nop");
    }
}


void interactive_test(){
    kprintf("\n\r\n\r\n\r### Interaktives Unterprogramm aufgerufen ###\n\r");
    kprintf("### Bitte nur Tastendruck, wenn idle! Danke ###\n\r");
    while(1){
        char c = uart_receive();
        int i;
        for (i = 0; i < c; i++) {
            kprintf("%c", c);
            wait();
        }
    }
}
void force_interrupts (char c) {
    switch (c) {
        case 's':
            asm("swi 99");
            break;
        case 'a':
            force_dataab();
            break;
        case 'u':
            asm("udf");
            break;
        case 'c':
            register_checker();
            break;
        case 'd':
            if (debug_irq == 0){
                debug_irq = 1;
            }else{
                debug_irq = 0;
            }
            break;
        case 'e':
            interactive_test();
            break;
        default:
            break;
    }
}

/* aktiviert Timer- und UART-Interrupts*/
void enable_IRQ_interrupts() {
    arm_interrupt_reg->EN_BASIC_IRQS = (1 << IRQ_TIMER_SHIFT);  // 0
    arm_interrupt_reg->EN_IRQ_2 = (1 << IRQ_UART_SHIFT);        // 25
    en_uart_interrupt();
    kprintf("\n\rEnable_ext_Interrupts\n\r");
}

/* print ! bei Timer-Interrupt */
uint32_t recognize_irq_interrupt(uint32_t stackadress, uint32_t spsr) {
    if (((arm_interrupt_reg->IRQ_BASIC_PENDING & (1 << IRQ_TIMER_SHIFT))>>IRQ_TIMER_SHIFT) == 1) {
        kprintf("!\n\r"); // print ! on timer interrupt
        uint32_t cpsr = swap_thread(stackadress, spsr);
        clear_timer();
        return cpsr;
    }
    if (((arm_interrupt_reg->IRQ_PENDING_2 & (1 << IRQ_UART_SHIFT))>>IRQ_UART_SHIFT) == 1) {
        kprintf("UART INTERRUPT\n\r");
        void(* interactive_testPtr)();
        interactive_testPtr = &interactive_test;
        prepare_thread(interactive_testPtr, (void*)NO_STACK_ADRESS, 0, 0);
        return 0x0;
    }
    else{
        kprintf("Intterrupt is broken, not timer not UART->return 0");
        return 0x0;
    }
}

