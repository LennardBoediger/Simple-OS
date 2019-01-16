#include <stddef.h>
#include "../include/init_thread.h"
#include "../user/include/uart_listen.h"
#include "../include/interrupt_regs_driver.h"
#include "../include/init_thread.h"
#include "../include/kprintf.h"
#include "../include/MMU.h"
#include "../include/process.h"

uint8_t no_ext_userprog;

uint8_t get_no_ext_userprog() {
    return no_ext_userprog;
}

extern void uart_listen();
__attribute__((weakref("uart_listen"))) static void new_uart_listen();


void prepare_uart_listenthread () {
    if (new_uart_listen == NULL) {
        no_ext_userprog = 1;
        kprintfln("### User Programm <uart_listen> nicht eingebunden ###");
    } else {
        no_ext_userprog = 0;
        void(*uart_listen_Ptr);
        uart_listen_Ptr = &new_uart_listen;
        prepare_thread(uart_listen_Ptr, (void *) NO_STACK_ADRESS, 0);
        kprintfln("\n\rHello :) \n\r");
        kprintfln("Drücke eine beliebige Taste, um einen interaktiven Test auszuführen...");
    }
}

void boot_system() {
    kprintfln("STARTING BOOT_SYSTEM");
    init_mmu();
    enable_IRQ_interrupts();
    init_tcbs();
    kprintfln("111111111111111111111111");
    init_process();
    kprintfln("222222222222222222222222");
    prepare_uart_listenthread();
    kprintfln("333333333333333333333333");
    prepare_idle_thread();
    kprintfln("444444444444444444444444");
    initialize_timer();
    kprintfln("555555555555555555555555");
    wait_for_first_irq();
}

