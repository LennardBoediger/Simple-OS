
#ifndef BS_PRAK_INIT_THREAD_H
#define BS_PRAK_INIT_THREAD_H
#define MAX_THREADS 32
#define IDLE_THREAD 32 // IDLE_THREAD has to be MAX_THREAD
#define BEENDET 0
#define WARTEND 1
#define BEREIT 2
#define LAUFEND 3
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

static volatile
struct tcb threads[MAX_THREADS];

int16_t prepare_thread(uint32_t pc, uint32_t* irq_stack_data, uint32_t irq_stack_data_size);

#endif //BS_PRAK_INIT_THREAD_H
