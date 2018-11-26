
#include <stdint.h>
#include "../../include/init_thread.h"
#include "../../include/threads_handler.h"
#include "../../include/kprintf.h"

//TODO: wie sollen vordere Bits sein bzw E-Bit (9. Stelle)
#define DEF_USERMODE_CPSR 16 //0x2D0
//in header:    - struct TCB (r0-r15, CPSR, Zusatzdaten-Stack-Pointer);
//               --> R0-R12 + R13 und R14 (aus USER-Mode) + R15 (im LR vom IRQ) + CPSR (im SPSR vom IRQ)
//              - Zusatzdaten Stack_max-size (gibt an, wie groß der Zusatzstack eines Threads sein darf
//enum threadzustand {beendet = 0, wartend = 1, bereit = 2, laufend = 3};

struct tcb threads[MAX_THREADS];

struct tcb* get_tcb(int32_t index) {
//    kprintf("AUS get_tcb -> PRINT_LR() -> threads[IDLE_THREAD].r5 = %x\n\r", threads[IDLE_THREAD].r5);
//    kprintf("AUS get_tcb -> PRINT_LR() -> threads[IDLE_THREAD].r6 = %x\n\r", threads[IDLE_THREAD].r6);
    return &threads[index];
}


void wait_for_first_irq(){
    kprintf("\n\r\n\rINIT DONE! WAITING FOR FIRST IRQ...\n\r");
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
    kprintf("TCBVorbereitung abgeschlossen\n\r");
}



void idle_thread(void* voidPointerOfFame){
    kprintf("start idle thread 32\n\r");
    while (1) {
    /* so lange, bis interrupt */
    }
}

//auch wenn idle keine daten braucht, braucht er einen (stack pointer?) und zustand
//-> er liegt bei i= 32 und wird wie ein normaler thread behandelt. Dafür ruft die funktion
// init_idle prepare auf idle auf start_idle_thread
void prepare_idle_thread(){
    kprintf("PREPARE IDLE-THREAD\n\r");
    void(* idle_thread_Ptr)(void*);
    idle_thread_Ptr = &idle_thread;
    kprintf("PREPARE_IDLE_THREAD -> idle_thread_pointer = %x\n\r", idle_thread_Ptr);
    prepare_thread(idle_thread_Ptr, (void*)NO_STACK_ADRESS, 0, 1);
}

int32_t find_free_tcb(uint8_t force_idle) {
    int16_t tcb_number = 0;
    struct tcb* thread = get_tcb(tcb_number);
    if (force_idle == 0) {
        while ((*thread).zustand != BEENDET) {
            if (tcb_number == MAX_THREADS - 1) {
                kprintf("K1 THREAD FREI :(");
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
//(void*)((*thread).data_stack_pointer - irq_stack_data_size)
int32_t prepare_thread(void (*pc)(void*), uint32_t* irq_stack_data, uint32_t irq_stack_data_size, uint8_t force_idle) {
    kprintf("PREPARE_THREAD() -> pc = %x\n\r", (uint32_t) pc);
    int32_t tcb_number = find_free_tcb(force_idle);
    struct tcb* thread = get_tcb(tcb_number);
    thread->lr_irq = (uint32_t) pc;
    uint16_t j;
//    for (j = 0; j < irq_stack_data_size; j++) {                 // copy data to thread stack
//        *(uint32_t*)((128*1024*(1018 - tcb_number))-j) = *(irq_stack_data - j);   //TODO testen
//    }

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
    thread->sp = (thread->data_stack_pointer - aligned_size); //TODO testen / fragen
    return tcb_number;
}