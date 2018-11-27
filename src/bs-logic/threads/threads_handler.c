#include "../../include/init_thread.h"
#include "../../include/kprintf.h"
#include "../../include/threads_handler.h"
#include <stdlib.h>

//#define INVALID_THREAD = 128;

uint16_t running_thread;
static uint32_t next_random = 1;

uint16_t get_running_thread(){
    return running_thread;
}


//in header:    - struct TCB (r0-r15, CPSR, Zusatzdaten-Stack-Pointer);
//               --> R0-R12 + R13 und R14 (aus USER-Mode) + R15 (im LR vom IRQ) + CPSR (im SPSR vom IRQ)
//              - Zusatzdaten Stack_max-size (gibt an, wie groß der Zusatzstack eines Threads sein darf

void print_lr (uint32_t lr){
    kprintf("Wert bei lr = %x\n\r", lr);
}


void save_thread(uint32_t stackadress, uint32_t spsr) {
    struct tcb* thread = get_tcb(running_thread);
    thread->r0 = *((uint32_t*) stackadress);
//    kprintf("Gesicherter IRQ stack inhalt auf R0: %x\n\r", *((uint32_t*) stackadress));
    thread->r1 = *((uint32_t*) stackadress+1);
//    kprintf("Gesicherter IRQ stack inhalt auf R1: %x\n\r", *((uint32_t*) stackadress+1));
    thread->r2 = *((uint32_t*) stackadress+2);
//    kprintf("Gesicherter IRQ stack inhalt auf R2: %x\n\r", *((uint32_t*) stackadress+2));
    thread->r3 = *((uint32_t*) stackadress+3);
//    kprintf("Gesicherter IRQ stack inhalt auf R3: %x\n\r", *((uint32_t*) stackadress+3));
    thread->r4 = *((uint32_t*) stackadress+4);
//    kprintf("Gesicherter IRQ stack inhalt auf R4: %x\n\r", *((uint32_t*) stackadress+4));
    thread->r5 = *((uint32_t*) stackadress+5);
//    kprintf("Gesicherter IRQ stack inhalt auf R5: %x\n\r", *((uint32_t*) stackadress+5));
    thread->r6 = *((uint32_t*) stackadress+6);
    thread->r7 = *((uint32_t*) stackadress+7);
    thread->r8 = *((uint32_t*) stackadress+8);
    thread->r9 = *((uint32_t*) stackadress+9);
    thread->r10 = *((uint32_t*) stackadress+10);
    thread->r11 = *((uint32_t*) stackadress+11);
    thread->r12 = *((uint32_t*) stackadress+12);
    thread->lr_irq = *((uint32_t*) stackadress+14);     //lr_IRQ //14
    thread->lr_usr = *((uint32_t*) stackadress+16);     //lr_usr //16
    kprintf("SAVE_THREAD -> lr_usr = %x, stack = %x\n\r", thread->lr_usr, *((uint32_t*) stackadress+16));
    thread->sp = *((uint32_t*) stackadress+17);     //sp_usr //17
//    kprintf("Gesicherter IRQ stack inhalt auf lr_usr: %x\n\r", *((uint32_t*) stackadress+16));
    kprintf("thread saved (save_thread)\n\r");
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
        if (thread->zustand == BEREIT || thread->zustand == LAUFEND) {
            number_of_active_threads++;
            active_threads[number_of_active_threads-1] = i;
        }
    }
    if(number_of_active_threads == 0) {
        kprintf("\n\r\n\r++++++++ NEUER THREAD => IDLE_THREAD ++++++++\n\r\n\r");
        return IDLE_THREAD;
    }
    uint32_t random_index = (own_random()%(number_of_active_threads));
    kprintf("\n\r\n\r++++++++ NEUER THREAD => %i ++++++++\n\r\n\r", active_threads[random_index]);
    return active_threads[random_index];
}


uint32_t load_thread(uint8_t next_thread, uint32_t irq_stackadress) {
    struct tcb* thread = get_tcb(next_thread);
//    kprintf("LOAD_THREAD -> NEXT_THREAD = %u\n\r", next_thread);
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
    kprintf("LOAD_THREAD -> lr_usr = %x, stack = %x\n\r", thread->lr_usr, *((uint32_t*) irq_stackadress+16));
    *((uint32_t*) irq_stackadress+17) = thread->sp; //15
//    kprintf("LOAD_THREAD -> Wert an lr_usr (irq-Stack) = %x\n\r", *((uint32_t*) irq_stackadress+16));
//    kprintf("LOAD_THREAD -> THREADS[NEXT_THREAD].lr_usr = %x\n\r", thread->lr_usr);
//    kprintf("LOAD_THREAD -> Wert an lr_irq (irq-Stack) = %x\n\r", *((uint32_t*) irq_stackadress+14));
//    kprintf("LOAD_THREAD -> THREADS[NEXT_THREAD].lr_irq = %x\n\r", thread->lr_irq);
    kprintf("thread loaded (load_thread)\n\r");
    return thread->cpsr;

}

uint32_t swap_thread(uint32_t irq_stackadress, uint32_t spsr) {
    kprintf("1 start swap_thread\n\r");
    struct tcb* idle_thread = get_tcb(IDLE_THREAD);
    struct tcb* old_running_thread = get_tcb(running_thread);
    if(idle_thread->zustand == BEENDET) { //wird beim initialen durchlauf aufgerufen
        kprintf("2 initial case (swap_thread)\n\r");
        running_thread = IDLE_THREAD;           // IDLE_thread ist erster thread der läuft
    }else {
        kprintf("3.1 default case (swap_thread)\n\r");
        if (old_running_thread->zustand == LAUFEND) {
            save_thread(irq_stackadress, spsr);
            old_running_thread->zustand = BEREIT;
            kprintf("3.2 thread is saved (swap_thread)\n\r");
        }
        running_thread = find_next_thread();
        kprintf("3.3 next thread found swap_thread\n\r");
    }
    struct tcb* next_running_thread = get_tcb(running_thread);
    next_running_thread->zustand = LAUFEND;
    kprintf("ZUSTAND: %i\n\r", next_running_thread->zustand);
    return load_thread(running_thread, irq_stackadress);
}
