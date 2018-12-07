#include "../include/uart_driver.h"
#include "../include/printf_lib.h"
#include "../include/Interrupt_Handler.h"
#include "../include/dataab_helper.h"
#include "../include/regcheck.h"
#include "../include/interrupt_regs_driver.h"
#include "../include/threads_handler.h"
#include "../include/init_thread.h"
#include "../user/include/interactive_test.h"

#define IRQ_FIQ_REG_OFFSET (0x7E00B000 - 0x3F000000 + 0x200)
#define TIMER_BASE (0x7E00B000 - 0x3F000000 + 0x400) //timerbaseadress minus MMU-offset
#define PREDIVIDER_VALUE 249 /* +1 = 250*/
#define INTERRUPT_TIMER 90000  /* HIER HAEUFIGKEIT DER INTERRUPTS EINSTELLBAR */
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



/* aktiviert Timer- und UART-Interrupts*/
void enable_IRQ_interrupts() {
    arm_interrupt_reg->EN_BASIC_IRQS = (1 << IRQ_TIMER_SHIFT);  // 0
    arm_interrupt_reg->EN_IRQ_2 = (1 << IRQ_UART_SHIFT);        // 25
    en_uart_interrupt();
    kprintf("\n\rEnable_ext_Interrupts\n\r");
}


uint32_t recognize_irq_interrupt(uint32_t irq_stackadress, uint32_t spsr) {
    if (((arm_interrupt_reg->IRQ_BASIC_PENDING & (1 << IRQ_TIMER_SHIFT))>>IRQ_TIMER_SHIFT) == 1) {
//        kprintf("!\n\r");       // print ! on timer interrupt
        int i = 0;
        struct tcb* thread;
        for (i = 0; i < MAX_THREADS; i++) {
            thread = get_tcb(i);
            if (thread->zustand == WARTEND) {
                if (thread->wartezeit > 0) {
                    thread->wartezeit--;
                } else if (thread->wartezeit == 0) {
                    thread->zustand = BEREIT;
                } else {
                    kprintfln("FIND_NEXT_THREAD -> thread->zustand == WARTEND aber thread->wartezeit < 0!? Darf nie sein");
                }
            }
        }
        uint32_t new_spsr = swap_thread(irq_stackadress, spsr);
        clear_timer();
        return new_spsr;
    }
    if (((arm_interrupt_reg->IRQ_PENDING_2 & (1 << IRQ_UART_SHIFT))>>IRQ_UART_SHIFT) == 1) {
        uart_receive();
        return 0x0;
    }
    else {
        kprintf("Intterrupt is broken, not timer not UART->return 0");
        return 0x0;
    }
}