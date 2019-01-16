#include "../../include/init_thread.h"
#include "../../include/printf_lib.h"
#include "../../include/threads_handler.h"
#include "../../include/process.h"
#include <stdlib.h>


uint32_t running_thread;
static uint32_t next_random = 1;


uint32_t get_running_thread(){
    return running_thread;
}


/* Speichert aktuellen Thread für Wechsel ins entsprechende TCB */
void save_thread(uint32_t stackadress, uint32_t spsr) {
    struct tcb* thread = get_tcb(get_running_thread());
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

/* RANDOM-FUNKTION */
/**/ uint32_t own_random_r(uint32_t *seed) {
/**/     *seed = *seed * 1103515245 + 12345;
/**/     return (*seed % ((uint32_t)RAND_MAX + 1));
/**/ }
/**/ uint32_t own_random(void) {
/**/     return (own_random_r(&next_random));
/**/ }
/**/ void own_s_random(uint32_t seed) {
/**/     next_random = seed;
/**/ }
/* ENDE RANDOM-FUNKTION */


/* Nächsten Thread finden und Wartezeit der Threads aktualisieren */
uint32_t find_next_thread() {
    uint8_t i = 0;
    uint8_t active_threads[MAX_THREADS];
    uint32_t number_of_active_threads = 0;
    // Geht alle TCBs durch und überprüft den Status der Threads
    for (i = 0; i < MAX_THREADS-1; i++) {
        struct tcb* thread = get_tcb(i);
        switch (thread->zustand) {
            // Wenn BEREIT oder (der Vollständigkeit wg) LAUFEND -> füge Index in active_threads[] hinzu
            case BEREIT:
            case LAUFEND:
                number_of_active_threads++;
                active_threads[number_of_active_threads-1] = i;
                break;
            // Wenn WARTEND -> Wartezeit aktualisieren
            case WARTEND:
                break;
            case BEENDET:
                break;
            default:
                kprintfln("HUPS! FIND_NEXT_THREAD -> INVALIDER ZUSTAND");
        }
    }
    if(number_of_active_threads == 0) {
//        kprintf("\n\r\n\r++++++++ NEXT THREAD => IDLE_THREAD ++++++++\n\r\n\r");
        return IDLE_THREAD;
    }
    // NÄCHSTEN THREAD AUSWÄHLEN
    uint32_t random_index = (own_random()%(number_of_active_threads));
//    kprintf("\n\r\n\r++++++++ NEXT THREAD => %i ++++++++\n\r\n\r", active_threads[random_index]);
    return active_threads[random_index];
}


/* Lädt nächsten Thread (für Interrupt-Handler) in den STACK */
uint32_t load_thread(uint32_t next_thread, uint32_t irq_stackadress) {
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
    kprintf("#######1 start swap_thread\n\r");
    struct tcb* idle_thread = get_tcb(IDLE_THREAD);
    struct tcb* old_running_thread = get_tcb(get_running_thread());
    // Nur bei SYSTEMSTART
    if(idle_thread->zustand == BEENDET) {
        running_thread = IDLE_THREAD;
    } else {
        // AKTUELLER THREAD WURDE UNTERBROCHEN
        if (old_running_thread->zustand == LAUFEND) {
            save_thread(irq_stackadress, spsr);
            old_running_thread->zustand = BEREIT;
        } else if(old_running_thread->zustand == WARTEND) {
            save_thread(irq_stackadress, spsr);
        }
        // Nächsten Thread AUSWÄHLEN (running_thread global)
        running_thread = find_next_thread();
    }
    kprintfln("SWAP_THREAD() -> VOR SWAP_PROCESS");
    // Wechselt (wenn nötig) in den dem Thread zugehörigen Prozess
    swap_process(get_tcb(running_thread)->process_id);
    kprintfln("SWAP_THREAD() -> NACH SWAP_PROCESS");
    // Nächsten Thread STARTEN
    struct tcb* next_running_thread = get_tcb(get_running_thread());
    next_running_thread->zustand = LAUFEND;
    return load_thread(get_running_thread(), irq_stackadress);
}
