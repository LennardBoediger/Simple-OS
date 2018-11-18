
#include <stdint.h>
#include "../../include/init_thread.h"
#include "../../include/threads_handler.h"
#include "../../include/kprintf.h"

//TODO: wie sollen vordere Bits sein bzw E-Bit (9. Stelle)
#define DEF_USERMODE_CPSR 0x2D0
//in header:    - struct TCB (r0-r15, CPSR, Zusatzdaten-Stack-Pointer);
//               --> R0-R12 + R13 und R14 (aus USER-Mode) + R15 (im LR vom IRQ) + CPSR (im SPSR vom IRQ)
//              - Zusatzdaten Stack_max-size (gibt an, wie groß der Zusatzstack eines Threads sein darf
enum threadzustand {beendet = 0, wartend = 1, bereit = 2, laufend = 3};

void init_threads(){
    uint8_t i;
    for (i = 0; i < 32; i++) {
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
        threads[i].lr = 0x14; //sollte nie...
        threads[i].pc = 0x15; //sollte nie zu sehen sein
        threads[i].cpsr = DEF_USERMODE_CPSR;
        threads[i].tcb_sp = 128*1024*(1018-i);
        threads[i].zustand = beendet;
    }
}

//TODO Datenübergeben?? - wenn wir es so machen, Datenalign?  threads[i].tcb_sp += data_size
void start_thread(uint32_t pc, uint32_t stack_data) {
    int i = 0;
    while (threads[i].zustand != beendet) {
        if (i == 31) {
            kprintf("K1 THREAD FREI :(");
            return;
        }
        i++;
    }
    //TODO DATEN IN STACK KOPIEREN
    threads[i].zustand = bereit;
    threads[i].lr = pc;
    threads[i].tcb_sp = 0; //TODO: hier noch Stackpointer aktualisieren
}



// init_threads() TCBs erstellen
// start_idle_thread()
