#include "../../include/init_thread.h"
#include "../../include/kprintf.h"
#include "../../include/threads_handler.h"
#include <stdlib.h>

//in header:    - struct TCB (r0-r15, CPSR, Zusatzdaten-Stack-Pointer);
//               --> R0-R12 + R13 und R14 (aus USER-Mode) + R15 (im LR vom IRQ) + CPSR (im SPSR vom IRQ)
//              - Zusatzdaten Stack_max-size (gibt an, wie groß der Zusatzstack eines Threads sein darf

//TODO NUR ZU DEBUG ZWECKEN:
void print_lr (int32_t lr) {
    kprintf("\n\r############");
    kprintf("\n\rlink register: %i\n\r", lr);
    kprintf("\n\r############");
    return;
}


void save_thread(uint32_t stackadress, uint32_t spsr) {
    threads[running_thread].r0 = *(uint32_t*) stackadress;
    threads[running_thread].r1 = *(uint32_t*) stackadress+4;
    threads[running_thread].r2 = *(uint32_t*) stackadress+4*2;
    threads[running_thread].r3 = *(uint32_t*) stackadress+4*3;
    threads[running_thread].r4 = *(uint32_t*) stackadress+4*4;
    threads[running_thread].r5 = *(uint32_t*) stackadress+4*5;
    threads[running_thread].r6 = *(uint32_t*) stackadress+4*6;
    threads[running_thread].r7 = *(uint32_t*) stackadress+4*7;
    threads[running_thread].r8 = *(uint32_t*) stackadress+4*8;
    threads[running_thread].r9 = *(uint32_t*) stackadress+4*9;
    threads[running_thread].r10 = *(uint32_t*) stackadress+4*10;
    threads[running_thread].r11 = *(uint32_t*) stackadress+4*11;
    threads[running_thread].r12 = *(uint32_t*) stackadress+4*12;
    threads[running_thread].sp = *(uint32_t*) stackadress+4*17;     //sp_usr
    threads[running_thread].lr_usr = *(uint32_t*) stackadress+4*16;     //lr_usr
    threads[running_thread].lr_irq = *(uint32_t*) stackadress+4*14;     //lr_IRQ
    threads[running_thread].cpsr = spsr;
}

uint8_t find_next_thread() {
    //TODO round robin
    uint8_t i = 1;
    while (threads[i].zustand != BEREIT) {
        if (i == MAX_THREADS-1) {
            kprintf("cant find Thread to execute -> loading idle");
            return IDLE_THREAD;
        }
        i++;
    }
    return i;
}

uint32_t load_thread(uint8_t next_thread, uint32_t irq_stackadress) {

    *(uint32_t*) irq_stackadress = threads[next_thread].r0;
    *(uint32_t*) (irq_stackadress+4) = threads[next_thread].r1;
    *(uint32_t*) (irq_stackadress+4*2) = threads[next_thread].r2;
    *(uint32_t*) (irq_stackadress+4*3) = threads[next_thread].r3;
    *(uint32_t*) (irq_stackadress+4*4) = threads[next_thread].r4;
    *(uint32_t*) (irq_stackadress+4*5) = threads[next_thread].r5;
    *(uint32_t*) (irq_stackadress+4*6) = threads[next_thread].r6;
    *(uint32_t*) (irq_stackadress+4*7) = threads[next_thread].r7;
    *(uint32_t*) (irq_stackadress+4*8) = threads[next_thread].r8;
    *(uint32_t*) (irq_stackadress+4*9) = threads[next_thread].r9;
    *(uint32_t*) (irq_stackadress+4*10) = threads[next_thread].r10;
    *(uint32_t*) (irq_stackadress+4*11) = threads[next_thread].r11;
    *(uint32_t*) (irq_stackadress+4*12) = threads[next_thread].r12;
    *(uint32_t*) (irq_stackadress+4*17) = threads[next_thread].sp;
    *(uint32_t*) (irq_stackadress+4*16) = threads[next_thread].lr_usr;
    *(uint32_t*) (irq_stackadress+4*14) = threads[next_thread].lr_irq;
    kprintf("thread loaded (load_thread)");
    return threads[next_thread].cpsr;

}

uint32_t swap_thread(uint32_t irq_stackadress, uint32_t spsr) {
    kprintf("1 start swap_thread\n\r");
    if(threads[IDLE_THREAD].zustand == BEENDET) { //wird beim initialen durchlauf aufgereufen
        kprintf("2 initial case (swap_thread)\n\r");
        running_thread = IDLE_THREAD;           // IDEL_thread ist erster thread der läuft
    }else{
        kprintf("3.1 default case (swap_thread)\n\r");
        save_thread(irq_stackadress, spsr);
        kprintf("3.2 thread is saved (swap_thread\n\r");
        threads[running_thread].zustand = BEREIT;
        running_thread = find_next_thread(); //TODO round robin
        kprintf("3.3 next thread found swap_thread\n\r");
    }
    threads[running_thread].zustand = LAUFEND;
    kprintf("ZUSTAND: %i\n\r", threads[running_thread].zustand);
    return load_thread(running_thread, irq_stackadress);
}
