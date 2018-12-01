#include <stdint.h>
#include "../../include/init_thread.h"
#include "../../include/threads_handler.h"
#include "../../include/kprintf.h"

#define DEF_USERMODE_CPSR 16 //0x2D0

struct tcb threads[MAX_THREADS];

struct tcb* get_tcb(int32_t index) {
    return &threads[index];
}


//TODO "SCHÖNES" BETRIEBSSYSTEM
void wait_for_first_irq(){
    kprintf("\n\rHello :) \n\r");
    kprintf("\n\rPress S for Software Interrupt \n\r");
    kprintf("Press A for Data Abort \n\r");
    kprintf("Press U for Undefined Instruction \n\r");
    kprintf("Press s to start Software Interrupt Threads\n\r");
    kprintf("Press a to start Data Abort Threads\n\r");
    kprintf("Press u to start Undefined Instruction Threads\n\r");
    kprintf("Press c to start RegChecker Threads\n\r");
    kprintf("Press something else to start interactive test Threads  \n\r");


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
        threads[i].data_stack_pointer = (uint32_t) (128*1024*(1018-i));
        threads[i].zustand = BEENDET;
    }
    kprintf("TCBVorbereitung abgeschlossen.\n\r");
}



void idle_thread(void* voidPointerOfFame){
    kprintf("Starte den idle thread (%i)...\n\r", IDLE_THREAD);
    while (1) {
    /* so lange, bis interrupt -> neuer thread*/
    }
}

void prepare_idle_thread(){
    void(* idle_thread_Ptr)(void*);
    idle_thread_Ptr = &idle_thread;
    prepare_thread(idle_thread_Ptr, (void*)NO_STACK_ADRESS, 0, 1);
}

//TODO: KERNEL PANIC DURCH THREADMANGEL FIXEN
int32_t find_free_tcb(uint8_t force_idle) {
    int16_t tcb_number = 0;
    struct tcb* thread = get_tcb(tcb_number);
    if (force_idle == 0) {
        while ((*thread).zustand != BEENDET) {
            if (tcb_number == MAX_THREADS - 1) {
                kprintf("K1 THREAD FREI :'(\n\r programm muss neu gestartet werden...\n\r"); /*EINGABE WIRD VERWORFEN*/
                return -1;
            }
            tcb_number++;
            thread = get_tcb(tcb_number);
        }
        (*thread).zustand = BEREIT;
    } else{
        tcb_number = IDLE_THREAD;
        thread = get_tcb(tcb_number);
        thread->zustand = BEENDET;
    }
    return tcb_number;
}

//TODO auslagern
void memcopy(void* src, void* dst, uint32_t irq_stack_data_size) {
    uint8_t *src_byte = (uint8_t *) src;
    uint8_t *dst_byte = (uint8_t *) dst;
    uint16_t j;
    for (j = 0; j < irq_stack_data_size; j++) {
        *dst_byte = *src_byte;
        src_byte++;
        dst_byte++;
    }
}

int32_t prepare_thread(void (*pc)(void*), void* irq_stack_data, uint32_t irq_stack_data_size, uint8_t force_idle) {
    int32_t tcb_number = find_free_tcb(force_idle);
    //Wenn kein Thread mehr frei war -> -1
//    if (tcb_number == -1) {
    struct tcb *thread = get_tcb(tcb_number);
    thread->lr_irq = (uint32_t) pc;
    memcopy(irq_stack_data, (uint32_t *) thread->data_stack_pointer, irq_stack_data_size);
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
    return tcb_number;
//    }
//    return -1;
}