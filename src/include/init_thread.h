
#ifndef BS_PRAK_INIT_THREAD_H
#define BS_PRAK_INIT_THREAD_H
#define MAX_THREADS 33
#define IDLE_THREAD 32 // IDLE_THREAD has to be MAX_THREAD
#define BEENDET 0
#define WARTEND 1
#define BEREIT 2
#define LAUFEND 3
#define NO_STACK_ADRESS 0x07b40000 //Stack des nie aktiv genutzten Idle-threads
#include <stdint.h>


struct tcb {
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
    uint32_t r12;
    uint32_t sp;
    uint32_t lr_usr;
    uint32_t lr_irq;
    uint32_t cpsr;
    uint32_t data_stack_pointer;
    uint8_t zustand;
};

struct tcb* get_tcb(int32_t index);


int32_t prepare_thread(void (*pc)(void*), uint32_t* irq_stack_data, uint32_t irq_stack_data_size, uint8_t force_idle);

void init_tcbs();
void prepare_idle_thread();


#endif //BS_PRAK_INIT_THREAD_H
