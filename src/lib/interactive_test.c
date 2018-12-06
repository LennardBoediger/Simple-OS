
#include "../include/interactive_test.h"
#include "../include/interrupt_regs_driver.h"
#include "../include/regcheck.h"
#include "../include/printf_lib.h"
//TODO: IN User-Ordner verlegen
#include "../include/syscalls.h"


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
        kprintf("%c", temp);
        wait();
    }
}

void interactive_test_passive(char c){
    int i;
    char temp = c;
    for (i = 0; i < temp; i++) {
        kprintf("%c", temp);
        wait();
    }
}

//TODO: IN User-Ordner verlegen
void user_thread_active(void* stack_pointer) {
    char input = *((char*) stack_pointer);
    switch(input) {
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
        default:
            interactive_test_active(input);
            break;
    }
    syscall_kill_thread();
    kprintf("\n\r\n\r\n\r\n\rDEAD THREADS CANNOT KPRINTF!!!!!11!!!elf!!!\n\r\n\r\n\r\n\r");
}

void user_thread_passive(void* stack_pointer) {
    char input = *((char*) stack_pointer);
    switch(input) {
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
        default:
            interactive_test_passive(input);
            break;
    }
    syscall_kill_thread();
    kprintf("\n\r\n\r\n\r\n\rDEAD THREADS CANNOT KPRINTF!!!!!11!!!elf!!!\n\r\n\r\n\r\n\r");
}