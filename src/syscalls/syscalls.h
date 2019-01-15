#ifndef BS_PRAK_SYSCALLS_H
#define BS_PRAK_SYSCALLS_H

#include <stdint.h>

void syscall_kill_thread();
void syscall_sleep_thread();
void syscall_prepare_thread(void (*pc)(void*), void* irq_stack_data, uint32_t irq_stack_data_size);
void syscall_prepare_process(void (*pc)(void*), void* irq_stack_data, uint32_t irq_stack_data_size);
void syscall_uart_print(char c);
uint32_t syscall_uart_read();


#endif //BS_PRAK_SYSCALLS_H
