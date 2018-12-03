#include "../../include/init_thread.h"
#include "../../include/printf_lib.h"
#include "../../include/threads_handler.h"
#include <stdlib.h>


uint32_t running_thread;
static uint32_t next_random = 1;


uint32_t get_running_thread(){
    return running_thread;
}



void save_thread(uint32_t stackadress, uint32_t spsr) {
    struct tcb* thread = get_tcb(running_thread);
    thread->r0 = *((uint32_t*) stackadress);
    thread->r1 = *((uint32_t*) stackadress+1);
    thread->r2 = *((uint32_t*) stackadress+2);
    thread->r3 = *((uint32_t*) stackadress+3);
    thread->r4 = *((uint32_t*) stackadress+4);
    thread->r5 = *((uint32_t*) stackadress+5);
    thread->r6 = *((uint32_t*) stackadress+6);
    thread->r7 = *((uint32_t*) stackadress+7);
    thread->r8 = *((uint32_t*) stackadress+8);
    thread->r9 = *((uint32_t*) stackadress+9);
    thread->r10 = *((uint32_t*) stackadress+10);
    thread->r11 = *((uint32_t*) stackadress+11);
    thread->r12 = *((uint32_t*) stackadress+12);
    thread->lr_irq = *((uint32_t*) stackadress+14);     //lr_IRQ
    thread->lr_usr = *((uint32_t*) stackadress+16);     //lr_usr
    thread->sp = *((uint32_t*) stackadress+17);     //sp_usr
    thread->cpsr = spsr;
}


uint32_t own_random_r(uint32_t *seed) {
    *seed = *seed * 1103515245 + 12345;
    return (*seed % ((uint32_t)RAND_MAX + 1));
}
uint32_t own_random(void) {
    return (own_random_r(&next_random));
}
void own_s_random(uint32_t seed) {
    next_random = seed;
}


uint32_t find_next_thread() {
    uint8_t i = 0;
    uint8_t active_threads[MAX_THREADS];
    uint32_t number_of_active_threads = 0;
    for (i = 0; i < MAX_THREADS-1; i++) {
        struct tcb* thread = get_tcb(i);
        switch (thread->zustand) {
            case BEREIT:
            case LAUFEND:
                number_of_active_threads++;
                active_threads[number_of_active_threads-1] = i;
                break;
            case WARTEND:
                if (thread->wartezeit > 0) {
                    thread->wartezeit--;
                } else if (thread->wartezeit == 0) {
                    thread->zustand = BEREIT;
                } else {
                  kprintfln("FIND_NEXT_THREAD -> thread->zustand == WARTEND aber thread->wartezeit < 0 !?!?! Darf nicht sein");
                }
                break;
            case BEENDET:
                break;
            default:
                kprintfln("HUPS! FIND_NEXT_THREAD -> INVALID ZUSTAND");
        }
    }
    if(number_of_active_threads == 0) {
//        kprintf("\n\r\n\r++++++++ NEXT THREAD => IDLE_THREAD ++++++++\n\r\n\r");
        return IDLE_THREAD;
    }
    uint32_t random_index = (own_random()%(number_of_active_threads));
//    kprintf("\n\r\n\r++++++++ NEXT THREAD => %i ++++++++\n\r\n\r", active_threads[random_index]);
    return active_threads[random_index];
}


uint32_t load_thread(uint8_t next_thread, uint32_t irq_stackadress) {
    struct tcb* thread = get_tcb(next_thread);
    *((uint32_t*) irq_stackadress) = thread->r0;
    *((uint32_t*) irq_stackadress+1) = thread->r1;
    *((uint32_t*) irq_stackadress+2) = thread->r2;
    *((uint32_t*) irq_stackadress+3) = thread->r3;
    *((uint32_t*) irq_stackadress+4) = thread->r4;
    *((uint32_t*) irq_stackadress+5) = thread->r5;
    *((uint32_t*) irq_stackadress+6) = thread->r6;
    *((uint32_t*) irq_stackadress+7) = thread->r7;
    *((uint32_t*) irq_stackadress+8) = thread->r8;
    *((uint32_t*) irq_stackadress+9) = thread->r9;
    *((uint32_t*) irq_stackadress+10) = thread->r10;
    *((uint32_t*) irq_stackadress+11) = thread->r11;
    *((uint32_t*) irq_stackadress+12) = thread->r12;
    *((uint32_t*) irq_stackadress+14) = thread->lr_irq; //13
    *((uint32_t*) irq_stackadress+16) = thread->lr_usr; //14
    *((uint32_t*) irq_stackadress+17) = thread->sp; //15
    return thread->cpsr;

}

uint32_t swap_thread(uint32_t irq_stackadress, uint32_t spsr) {
//    kprintf("1 start swap_thread\n\r");
    struct tcb* idle_thread = get_tcb(IDLE_THREAD);
    struct tcb* old_running_thread = get_tcb(running_thread);
    if(idle_thread->zustand == BEENDET) { //wird beim initialen durchlauf aufgerufen
//        kprintf("2 initial case (swap_thread)\n\r");
        running_thread = IDLE_THREAD;           // IDLE_thread ist erster thread der läuft
    }else {
//        kprintf("3.1 default case (swap_thread)\n\r");
        if (old_running_thread->zustand == LAUFEND) {
            save_thread(irq_stackadress, spsr);
            old_running_thread->zustand = BEREIT;
//            kprintf("3.2 thread is saved (swap_thread)\n\r");
        }
        running_thread = find_next_thread();
//        kprintf("3.3 next thread found swap_thread\n\r");
    }
    struct tcb* next_running_thread = get_tcb(running_thread);
    next_running_thread->zustand = LAUFEND;
    return load_thread(running_thread, irq_stackadress);
}
