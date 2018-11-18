
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

void init_tcbs(){
    uint8_t i;
    for (i = 0; i < MAX_THREADS; i++) {
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
        threads[i].sp = 0x13; //sollte nie...
        threads[i].lr_usr = 0x14; //sollte nie...
        threads[i].lr_irq = 0x15; //sollte nie zu sehen sein
        threads[i].cpsr = DEF_USERMODE_CPSR;
        threads[i].data_stack_pointer = 128*1024*(1018-i);
        threads[i].zustand = BEENDET;
    }
}
void start_idle_thread(){
    kprintf("TODO start thread 32");
}
//TODO Datenübergeben?? - wenn wir es so machen, Datenalign?  threads[i].tcb_sp += data_size
int16_t prepare_thread(uint32_t pc, uint32_t* irq_stack_data, uint32_t irq_stack_data_size) {
    int16_t i = 0;
    while (threads[i].zustand != BEENDET) {
        if (i == MAX_THREADS-1) {
            kprintf("K1 THREAD FREI :(");
            return -1;
        }
        i++;
    }
    //TODO DATEN IN STACK KOPIEREN
    threads[i].zustand = BEREIT;
    threads[i].lr_usr = pc;
    threads[i].data_stack_pointer = 0; //TODO: hier noch Stackpointer aktualisieren
    return i;
}