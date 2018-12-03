#include "../include/printf_lib.h"
#include "../include/syscall_helper.h"


void syscall_kill_thread(){
    kprintfln("SYSCALL KILL_THREAD was called!!");
    asm("swi 0"); // swi SYS_KILL_THREAD
}
void syscall_prepare_thread(void (*pc)(void*), void* irq_stack_data, uint32_t irq_stack_data_size, uint8_t force_idle){
    asm("swi 1"); //swi SYS_PREPARE_THREAD
}

void syscall_sleep_thread() {
    asm("swi 2");
}

uint32_t syscall_uart_read() {
    asm("swi 4");
    uint32_t r8 = read_r8_char();
    kprintfln("value from read_r8_char = %i", r8);
    //im Userfunktionsaufruf syscall_uart_read: r8 in eine Variable laden
    return r8;
}