#include <stddef.h>
#include <stdint.h>
#include "../../include/init_thread.h"
#include "../../include/threads_handler.h"
#include "../../include/printf_lib.h"
#include "../../include/Boot.h"
#include "../../include/systemfunctions.h"
#include "../../include/process.h"
#include "../../include/Rick.h"
#include "../../include/MMU.h"

#define DEF_USERMODE_CPSR 16 //0x2D0

struct tcb threads[MAX_THREADS];



uint8_t force_idle;

struct tcb* get_tcb(int32_t index) {
    return &threads[index];
}


void wait_for_first_irq(){
    kprintfln("------------------------------------------------------");
    while(1){
    }
}

void init_tcbs(){
    uint8_t i;
    for (i = 0; i <= (MAX_THREADS-1); i++) {
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
        threads[i].data_stack_pointer = (uint32_t) (126*1024*1023-i*1024); // 0x07DE 0800 TODO TODO TODO
        threads[i].zustand = BEENDET;
        threads[i].wartezeit = -1;
        threads[i].process_id = -1;
    }
    kprintf("TCBVorbereitung abgeschlossen.\n\r");
}

extern void idle_thread();
__attribute__((weakref("idle_thread"))) static void new_idle_thread(void* voidPointerOfFame);


void prepare_idle_thread(){
    force_idle = 1;
    void(* idle_thread_Ptr)(void*);
    idle_thread_Ptr = &new_idle_thread;
    prepare_thread(idle_thread_Ptr, (void*)NO_STACK_ADRESS, 0);
}

int32_t find_free_tcb() {
    int16_t tcb_number = 0;
    struct tcb* thread = get_tcb(tcb_number);
    if (force_idle == 0) {
        while ((*thread).zustand != BEENDET) {
            if (tcb_number == MAX_THREADS - 1) {
                kprintf("\n\rK1 THREAD FREI :'(\n\r"); /*EINGABE WIRD VERWORFEN*/
                return -1;
            }
            tcb_number++;
            thread = get_tcb(tcb_number);
        }
        thread->zustand = BEREIT;
        thread->wartezeit = 0;
    } else{
        tcb_number = IDLE_THREAD;
        thread = get_tcb(tcb_number);
        thread->zustand = BEENDET;
        force_idle = 0;
    }
    return tcb_number;
}


void prepare_thread(void (*pc)(void*), void* irq_stack_data, uint32_t irq_stack_data_size) {
    int i = 0;
    uint8_t nr_of_running_threads_in_unborn_process = 0;
    for (i = 0; i < MAX_THREADS; i++) {
        if (get_tcb(i)->zustand != BEENDET && get_tcb(i)->process_id == get_unborn_process()) {
            nr_of_running_threads_in_unborn_process++;
        }
    }
    int32_t tcb_number = find_free_tcb();
    struct tcb *thread = get_tcb(tcb_number);
    if (nr_of_running_threads_in_unborn_process == 0) {
        // Thread wird in einem neuen Prozess erzeugt
        thread->process_id = get_unborn_process();
        kprintfln("+++ NEUER PROZESS %i +++", thread->process_id);
    } else {
        thread->process_id = get_current_process();
        kprintfln("+++ NEUER THREAD - IN PROCESS %i +++", thread->process_id);
    }
    thread->lr_irq = (uint32_t) pc;
    memcopy(irq_stack_data, (uint32_t *) (thread->data_stack_pointer-0x07D00000+(get_phys_user_stacks(thread->process_id)*0x100000)), irq_stack_data_size);
    if (tcb_number == IDLE_THREAD) {
        thread->r0 = NO_STACK_ADRESS;
    } else {
        thread->r0 = thread->data_stack_pointer;
    }
    //Abfangen, wenn Größe des Stacks nicht mit dem Alignment zusammenpasst
    uint32_t aligned_size;
    if (irq_stack_data_size % 8 != 0) {
        aligned_size = irq_stack_data_size + (8 - (irq_stack_data_size % 8));
    } else {
        aligned_size = irq_stack_data_size;
    }
    //Stackpointer auf oberstes Elemtent des Stacks
    thread->sp = (thread->data_stack_pointer - aligned_size);
}