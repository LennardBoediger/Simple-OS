
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
    char temp = c;
    for (i = 0; i < 200; i++) {
        kprintf("%c", temp);
        wait();
    }
    kprintf("interactive test done \n\r");
}

void user_thread(void* stack_pointer) {
    char input = *((char*) stack_pointer);
//    int i, j;
//    char temp = input;
    kprintf("USER_THREAD LÄUFT\n\r");
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
/*            for (i = 0; i < 200; i++) {
                kprintf("%c", temp);
                for (j = 0; j < 52147; j++) {
                    asm("nop");
                }
            }
            kprintf("interactive test done \n\r");
*/            kprintf("interactive_test definitly done\n\r");
            break;
    }
    kprintf("\n\r+++ USERTHREAD DONE +++\n\r");
    asm("swi 42");
    kprintf("\n\r\n\r\n\r\n\rDEAD THREADS CANNOT KPRINTF!!!!!11!!!elf!!!\n\r\n\r\n\r\n\r");
}