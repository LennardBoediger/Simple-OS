#include <stddef.h>
#include "include/interactive_test.h"
#include "include/interactive_test_helper.h"
#include "include/uprintf.h"
#include "../syscalls/syscalls.h"


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

/*What to do:
        kprintfln("n: lesender Zugriff auf Null-Pointer.");
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
        kprintfln("Drücke eine andere Taste, um einen interaktiven Test auszuführen...");*/
void user_thread_passive(void* stack_pointer) {
    char input = *((char*) stack_pointer);
    switch(input) {
        case 'n':;
            volatile int* null = NULL;
            *null;
            uprintfln("Achtung: lesender Zugriff auf Null-Pointer Defekt");
            break;
        case 'p':
            branch_to_np();
            uprintfln("Achtung: Sprung auf Null-Pointer Defekt");
            break;
        case 'd':
            read_address((uint32_t*) 0x00300000);
            uprintfln("Achtung: lesender Zugriff auf Kernel Daten Defekt");
            break;
        case 'k':
            read_address((uint32_t*)0x00200000);
            uprintfln("Achtung: lesender Zugriff aufKernel Coder Defekt");
            break;
        case 'K':
            read_address((uint32_t*)(uint32_t*)0x07F);
            uprintfln("Achtung: lesender Zugriff auf Kernel Stack Defekt"); //TODO make this K
            break;
        case 'g':
            read_address((uint32_t*)(0x7E201000 - 0x3F000000));
            uprintfln("Achtung: lesender Zugriff auf IO Defekt");
            break;
        case 'c':
            write_address((uint32_t*) 0x0004);
            uprintf("Achtung: Schreiben auf eigenen code Defekt");
            break;
        case 's':
            stack_overflow(1337);
            uprintf("Achtung: Stack Overflow Defekt");
            break;
        case 'u':
            read_address((uint32_t*)0x00700000);
            uprintfln("Achtung: lesender Zugriff auf Unbound Defekt");
            break;
        case 'x':
            branchto((uint32_t*)stack_pointer);
            uprintfln("Achtung: Sprung auf eigene Daten Defekt");
            break;
        case 'a':
            asm("udf");
            break;
        default:
            interactive_test_passive(input);
            break;
    }
    syscall_kill_thread();
    uprintf("\n\r\n\r\n\r\n\rDEAD THREADS CANNOT KPRINTF!!!!!11!!!elf!!!\n\r\n\r\n\r\n\r");
}