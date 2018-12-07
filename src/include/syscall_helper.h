#ifndef BS_PRAK_SYSCALL_HELPER_H
#define BS_PRAK_SYSCALL_HELPER_H

#include <stdint-gcc.h>

uint32_t read_r8_char();
void write_r8_char(char to_transmit);
void write_r5_to_r7(void (*pc)(void*), void* irq_stack_data, uint32_t irq_stack_data_size);

#endif //BS_PRAK_SYSCALL_HELPER_H
