#include "../include/printf_lib.h"
#include "../include/interrupt_regs_driver.h"
#include "../include/Interrupt_printer.h"
#include "../include/mode_reg_helper.h"
#include "../include/dataab_helper.h"
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
#define WAITING_TIME 3

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
//TODO: put user sys call funtions in user code file

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
        kprintf("------------SYSCALL NUMBER: %x ---------------\n\r",swi_syscall_number);
        uint32_t to_transmit;
        switch (swi_syscall_number) {
            // Zustand des alten Threads = BEENDET, führt Thread-Wechsel durch und löscht den Timer
            case SYS_KILL_THREAD:
                kprintf("-----SYS_KILL_THREAD-----\n\r");
                get_tcb(get_running_thread())->zustand = BEENDET;
                next_spsr = swap_thread(swi_stackadress, spsr);
                clear_timer();
                break;
            //
            case SYS_PREPARE_THREAD:
                prepare_thread((void*)*((uint32_t*) swi_stackadress),(void*)*((uint32_t*) swi_stackadress+1),
                               *((uint32_t*) swi_stackadress+2),*((uint8_t*) swi_stackadress+3));
                //TODO: NEXT_SPSR = SWAP_THREAD() & CLEAR_TIMER() ?!
                break;
            case SYS_SLEEP_THREAD:
                get_tcb(get_running_thread())->zustand = WARTEND;
                get_tcb(get_running_thread())->wartezeit = WAITING_TIME;
                next_spsr = swap_thread(swi_stackadress, spsr);
                clear_timer();
                break;
            case SYS_UART_PRINT:
                to_transmit = *((uint32_t*) swi_stackadress+8);
                uart_transmit((char) to_transmit);
                break;
            case SYS_UART_READ:
                //ZUGRIFF AUF READ BUFFER->return Char, diesen in r8 (Stackadress+8)
                *((uint32_t*) swi_stackadress+8) = (uint32_t) read_uart_buffer();
                break;
            default:
                kprintfln("UNKNOWN SYSCALL... JETZT WIRDS KOMISCH");
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