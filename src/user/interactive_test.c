
#include "include/interactive_test.h"
#include "../include/interrupt_regs_driver.h"
#include "include/uprintf.h"
//TODO: IN User-Ordner verlegen
#include "../include/syscalls.h"
#include "../include/threads_handler.h"


/* erzeugt kurze Pausen zwischen den Buchstaben. 52147 = magic number */
static void wait() {
    int i;
    for (i = 0; i < 52147; i++) {
        asm("nop");
    }
}

void interactive_test_active(char c){
    int i;
    char temp = c;
    for (i = 0; i < temp; i++) {
        uprintf("%c", temp);
        wait();
    }
}

void interactive_test_passive(char c){
    int i;
    char temp = c;
    for (i = 0; i < temp; i++) {
        uprintf("%c", temp);
        syscall_sleep_thread();
    }
}


void user_thread_active(void* stack_pointer) {
    char input = *((char*) stack_pointer);
    interactive_test_active(input);
    syscall_kill_thread();
    uprintf("\n\r\n\r\n\r\n\rDEAD THREADS CANNOT KPRINTF!!!!!11!!!elf!!!\n\r\n\r\n\r\n\r");
}

void user_thread_passive(void* stack_pointer) {
    char input = *((char*) stack_pointer);
    switch(input) {
        case 's':
            asm("swi 99");
            break;
        case 'u':
            asm("udf");
            break;
        default:
            interactive_test_passive(input);
            break;
    }
    syscall_kill_thread();
    uprintf("\n\r\n\r\n\r\n\rDEAD THREADS CANNOT KPRINTF!!!!!11!!!elf!!!\n\r\n\r\n\r\n\r");
}