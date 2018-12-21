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
        kprintfln("\n\rHello :) \n\r");
        kprintfln("Drücke n: lesender Zugriff auf Null-Pointer.");
        kprintfln("Drücke p: Sprung auf Null-Pointer.");
        kprintfln("Drücke d: lesender Zugriff auf Kernel-Daten.");
        kprintfln("Drücke k: lesender Zugriff auf Kernel-Code.");
        kprintfln("Drücke K: lesender Zugriff auf Kernel-Stack.");
        kprintfln("Drücke g: lesender Zugriff auf Peripherie-Gerät, z.B. UART. ");
        kprintfln("Drücke c: schreibender Zugriff auf eigenen Code.");
        kprintfln("Drücke s: Stack-Overflow.");
        kprintfln("Drücke u: lesender Zugriff auf nicht zugeordnete Adresse. ");
        kprintfln("Drücke x: Sprung auf eigene Daten oder Stack.");
        kprintfln("Drücke a, um aus einem User-Thread eine 'Undefined Instruction' auszulösen");
        kprintfln("N: lesender Zugriff auf Null-Pointer.");
        kprintfln("P: Sprung auf Null-Pointer.");
        kprintfln("C: schreibender Zugriff auf eigenen Code.");
        kprintfln("U: lesender Zugriff auf nicht zugeordnete Adresse. X: Sprung auf User Code.");

        kprintfln("Drücke eine andere Taste, um einen interaktiven Test auszuführen...");
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

