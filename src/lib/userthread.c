
#include "../include/userthread.h"
#include "../include/interrupt_regs_driver.h"
#include "../include/regcheck.h"

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

void user_thread(void* stack_pointer) {
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
            interactive_test(input);
            asm("swi 42");
            break;
    }
}