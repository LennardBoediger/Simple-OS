#include "../include/printf_lib.h"
#include "../include/interrupt_regs_driver.h"
#include "../include/Interrupt_printer.h"
#include "../include/threads_handler.h"
#include "../include/init_thread.h"
#include "../include/Interrupt_Handler.h"
#include "../include/uart_driver.h"
#define SWI_PC_Offset (-4)
//Sys call defines
#define SYS_KILL_THREAD 0
#define SYS_PREPARE_THREAD 1
#define SYS_SLEEP_THREAD 2
#define SYS_UART_PRINT 3
#define SYS_UART_READ 4
#define WAITING_TIME 1

uint8_t debug_interrupt = 0;  //Global RINT IRQ


void reset(uint32_t stackadress, uint32_t cpsr, uint32_t spsr) {
    print_interrupt(stackadress, cpsr, spsr, "RESET", 0, 0);
}

uint32_t undef(uint32_t stackadress, uint32_t cpsr, uint32_t spsr) {
    print_interrupt(stackadress, cpsr, spsr, "UNDEFINED", -4, 0);
    //Wenn wir aus dem User_mode kommen, wurde es von einem Thread ausgelößt
    uint32_t next_spsr = spsr;
    if ((spsr % MODE_MASK) == USER_MODE) {
        get_tcb(get_running_thread())->zustand = BEENDET;
        next_spsr = swap_thread(stackadress, spsr);
        clear_timer();
    }
    return next_spsr;
}


/* bearbeitet den "swi xx"-call */
uint32_t swi_interrupt(uint32_t swi_stackadress, uint32_t cpsr, uint32_t spsr) {
    if (debug_interrupt == 1) {
        print_interrupt(swi_stackadress, cpsr, spsr, "SOFTWARE INTERRUPT", -4, 0);
    }
    //Wenn wir aus dem User_mode kommen, kam der SWI von einem Thread
    uint32_t next_spsr = spsr;
    if ((next_spsr % MODE_MASK) == USER_MODE) {
        //SYSCALL-NUMMER EINLESEN (swi xxx)
        uint32_t swi_syscall_number = *(uint32_t*)((*(uint32_t*) (swi_stackadress+14*4)) + SWI_PC_Offset) % 0x1000000;
        uint32_t to_transmit;
        uint32_t input;
        switch (swi_syscall_number) {
            // Zustand des alten Threads = BEENDET, führt Thread-Wechsel durch und löscht den Timer
            case SYS_KILL_THREAD:
                get_tcb(get_running_thread())->zustand = BEENDET;
                next_spsr = swap_thread(swi_stackadress, spsr);
                clear_timer();
                break;
            //
            case SYS_PREPARE_THREAD:
                prepare_thread((void*)*((uint32_t*) swi_stackadress),
                               //TODO: VALIDATE swi_sta...+1 (irq_stack_data)
                               (void*)*((uint32_t*) swi_stackadress+1),
                               *((uint32_t*) swi_stackadress+2));
                break;
            case SYS_SLEEP_THREAD:
                get_tcb(get_running_thread())->zustand = WARTEND;
                get_tcb(get_running_thread())->wartezeit = WAITING_TIME;
                next_spsr = swap_thread(swi_stackadress, spsr);
                clear_timer();
                break;
            case SYS_UART_PRINT:
                to_transmit = *((uint32_t*) swi_stackadress);
                uart_transmit((char) to_transmit);
                break;
            case SYS_UART_READ:
                //ZUGRIFF AUF READ BUFFER->return Char, diesen in r8 (Stackadress+8)
                input = (uint32_t) read_uart_buffer();
                // FÜR NÄCHSTE ABGABE RAUS!!
                if (input == 'S') {
                    kprintfln("\n\rS: Systemcall nicht möglich");
                    asm("swi 0");
                } else *((uint32_t*) swi_stackadress) = input;
                break;
            default:
                kprintfln("\n\rUNKNOWN SYSCALL...\n\r");
                break;
        }
    }
    return next_spsr;
}


uint32_t prefab(uint32_t stackadress, uint32_t cpsr, uint32_t spsr) {
    print_interrupt(stackadress, cpsr, spsr, "PREFETCH ABORT", -4, 0);
    //Wenn wir aus dem User_mode kommen, wurde es von einem Thread ausgelößt
    uint32_t next_spsr = spsr;
    if ((spsr % MODE_MASK) == USER_MODE) {
        get_tcb(get_running_thread())->zustand = BEENDET;
        next_spsr = swap_thread(stackadress, spsr);
        clear_timer();
    }
    return next_spsr;
}

uint32_t dataab(uint32_t stackadress, uint32_t cpsr, uint32_t spsr) {
    print_interrupt(stackadress, cpsr, spsr, "DATA ABORT", -8, 1);
    //Wenn wir aus dem User_mode kommen, wurde es von einem Thread ausgelößt
    uint32_t next_spsr = spsr;
    if ((spsr % MODE_MASK) == USER_MODE) {
        get_tcb(get_running_thread())->zustand = BEENDET;
        next_spsr = swap_thread(stackadress, spsr);
        clear_timer();
    }
    return next_spsr;
}

uint32_t irq(uint32_t irq_stackadress, uint32_t cpsr, uint32_t spsr) {
    if (debug_interrupt == 1){
        print_interrupt(irq_stackadress, cpsr, spsr, "IRQ INTERRUPT", -8, 0);
    }
    return recognize_irq_interrupt(irq_stackadress, spsr);
}

void fiq(uint32_t stackadress, uint32_t cpsr, uint32_t spsr) {
    print_interrupt(stackadress, cpsr, spsr, "DATA ABORT", -8, 0);
}