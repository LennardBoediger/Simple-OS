
#include <stdint.h>
#include "../../include/init_thread.h"
#include "../../include/threads_handler.h"
#include "../../include/kprintf.h"

//TODO: wie sollen vordere Bits sein bzw E-Bit (9. Stelle)
#define DEF_USERMODE_CPSR 0x2D0
//in header:    - struct TCB (r0-r15, CPSR, Zusatzdaten-Stack-Pointer);
//               --> R0-R12 + R13 und R14 (aus USER-Mode) + R15 (im LR vom IRQ) + CPSR (im SPSR vom IRQ)
//              - Zusatzdaten Stack_max-size (gibt an, wie groß der Zusatzstack eines Threads sein darf
//enum threadzustand {beendet = 0, wartend = 1, bereit = 2, laufend = 3};

void wait_for_first_irq(){
    kprintf("WAITING FOR FIRST IRQ");
    while(1){
    }
}

void init_tcbs(){
    uint8_t i;
    for (i = 0; i < MAX_THREADS; i++) {
        kprintf("TCBs werden vorbereitet\n\r");
        threads[i].r0 = 0x0;
        threads[i].r1 = 0x1;
        threads[i].r2 = 0x2;
        threads[i].r3 = 0x3;
        threads[i].r4 = 0x4;
        threads[i].r5 = 0x5;
        threads[i].r6 = 0x6;
        threads[i].r7 = 0x7;
        threads[i].r8 = 0x8;
        threads[i].r9 = 0x9;
        threads[i].r10 = 0x10;
        threads[i].r11 = 0x11;
        threads[i].r12 = 0x12;
        threads[i].sp = 0x666; //sollte nie...
        threads[i].lr_usr = 0x666; //sollte nie...
        threads[i].lr_irq = 0x666; //sollte nie zu sehen sein
        threads[i].cpsr = DEF_USERMODE_CPSR;
        threads[i].data_stack_pointer = (uint32_t) 128*1024*(1018-i);
        threads[i].zustand = BEENDET;
    }
    kprintf("finito\n\r");
}



void idle_thread(){
    kprintf("start idle thread 32");
    while (1) {
        kprintf("xyz");
    /* so lange, bis interrupt */
    }
}

//auch wenn idle keine daten braucht, braucht er einen (stack pointer?) und zustand
//-> er liegt bei i= 32 und wird wie ein normaler thread behandelt. Dafür ruft die funktion
// init_idle prepare auf idle auf start_idle_thread
void prepare_idle_thread(){
    void(* idle_thread_Ptr)();
    idle_thread_Ptr = &idle_thread;
    prepare_thread(idle_thread_Ptr, (void*)NO_STACK_ADRESS, 0, 1); //TODO: DONE?! chose a better pointer?
}


int16_t prepare_thread(void (*pc)(), uint32_t* irq_stack_data, uint32_t irq_stack_data_size, uint8_t force_idle) {
    int16_t i = 0;
    if (force_idle == 0) {
        while (threads[i].zustand != BEENDET) {
            if (i == MAX_THREADS - 1) {
                kprintf("K1 THREAD FREI :(");
                return -1;
            }
            i++;
        }
        threads[i].zustand = BEREIT;
    } else{
        i = IDLE_THREAD;
        threads[i].zustand = BEENDET;
    }
    //TODO DATEN IN STACK KOPIEREN
    threads[i].lr_usr = (uint32_t) pc;
    uint16_t j;
    for (j = 0; j < irq_stack_data_size; j++) {                 // copy data to thread stack
        *(uint32_t*)(128*1024*(1018 - j)) = *(irq_stack_data - j);   //TODO testen
    }
    if (i == IDLE_THREAD) {
        threads[i].data_stack_pointer = NO_STACK_ADRESS;
    } else threads[i].data_stack_pointer = (uint32_t) 128*1024*(1018-i);
    threads[i].sp = threads[i].data_stack_pointer - irq_stack_data_size;
    return i;
}