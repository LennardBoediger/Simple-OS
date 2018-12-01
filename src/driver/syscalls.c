#include "../include/kprintf.h"


void syscall_kill_thread(){
    asm("swi 0"); // swi SYS_KILL_THREAD
}
void syscall_prepare_thread(void (*pc)(void*), void* irq_stack_data, uint32_t irq_stack_data_size, uint8_t force_idle){
    asm("swi 1"); //swi SYS_PREPARE_THREAD
}

//TODO: braucht es einen Rückgabewert oder reicht es, bei "swi 4"-call r0 zu befüllen?
uint32_t syscall_uart_read() {
    asm("swi 4");
    return;
}