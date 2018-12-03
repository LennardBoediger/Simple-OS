
#include "../include/userthread.h"
#include "../include/interrupt_regs_driver.h"
#include "../include/regcheck.h"
#include "../include/printf_lib.h"


/* erzeugt kurze Pausen zwischen den Buchstaben. 52147 = magic number */
static void wait() {
    int i;
    for (i = 0; i < 52147; i++) {
        asm("nop");
    }
}


void interactive_test(char c){
    int i;
    char temp = c;
    for (i = 0; i < temp; i++) {
        kprintf("%c", temp);
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
            break;
    }
    asm("swi 42");
    kprintf("\n\r\n\r\n\r\n\rDEAD THREADS CANNOT KPRINTF!!!!!11!!!elf!!!\n\r\n\r\n\r\n\r");
}