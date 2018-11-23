#ifndef BS_PRAK_THREADS_HANDLER_H
#define BS_PRAK_THREADS_HANDLER_H


uint32_t load_thread(uint8_t next_thread, uint32_t irq_stackadress);
uint32_t swap_thread(uint32_t irq_stackadress, uint32_t spsr);

#endif //BS_PRAK_THREADS_HANDLER_H
