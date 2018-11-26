
#include "../include/userthread.h"
#include "../include/interrupt_regs_driver.h"
#include "../include/regcheck.h"
#include "../include/kprintf.h"

/* erzeugt kurze Pausen zwischen den Buchstaben. 52147 = magic number */
static void wait() {
    int i;
    for (i = 0; i < 52147; i++) {
        asm("nop");
    }
}


void interactive_test(char c){
    int i;
    for (i = 0; i < c; i++) {
        kprintf("%c", c);
        wait();
    }
}

void user_thread2(void* stack_pointer) {
//    char input = *((char*) stack_pointer);
    kprintf("USER_THREAD LÄUFT\n\r");
    char input = 'n';
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
            interactive_test(input);
            asm("swi 42");
            break;
    }
    while (1);
}