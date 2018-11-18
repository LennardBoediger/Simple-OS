#ifndef BS_PRAK_THREADS_HANDLER_H
#define BS_PRAK_THREADS_HANDLER_H

extern uint8_t running_thread;

uint32_t swap_thread(uint32_t stackadress, uint32_t spsr);

#endif //BS_PRAK_THREADS_HANDLER_H
