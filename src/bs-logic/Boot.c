#include <stddef.h>
#include "../include/init_thread.h"
#include "../user/include/uart_listen.h"
#include "../include/interrupt_regs_driver.h"
#include "../include/init_thread.h"
#include "../include/kprintf.h"
#include "../include/MMU.h"

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
        kprintf("\n\rHello :) \n\r");
        kprintf("\n\rDrücke S, um direkt aus der Interruptbehandlung einen bekannten Systemruf auszurufen\n\r");
        kprintf("Drücke s, um aus einem User-Thread einen unbekannten Systemruf aufzurufen\n\r");
        kprintf("Drücke u, um aus einem User-Thread eine 'Undefined Instruction' auszulösen\n\r");
        kprintf("Drücke eine andere Taste, um einen interaktiven Test auszuführen...\n\r");
    }
}

void boot_system() {
    kprintfln("STARTING BOOT_SYSTEM");
    init_mmu();
    kprintfln("BOOT_SYSTEM -> INIT_MMU DONE");
    enable_IRQ_interrupts();
    init_tcbs();
    prepare_uart_listenthread();
    prepare_idle_thread();
    initialize_timer();
    wait_for_first_irq();
}

