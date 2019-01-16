#include <stdint.h>
#include "../syscalls/syscalls.h"
#include "../include/printf_lib.h"
#include "include/uprintf.h"
#include "include/interactive_test.h"


void prepare_user_process(char input){
    static void(* user_process_Ptr)(void*);
    char c = input;
    user_process_Ptr = &user_process;
    syscall_prepare_process(user_process_Ptr, (void*) &c, sizeof(c));
}

void uart_listen(){
    while(1) {
        kprintfln("UART_LISTEN() -> 11111");
        char input = (char) syscall_uart_read();
        kprintfln("UART_LISTEN() -> 22222");
        while (input != 0) {
            prepare_user_process(input);
            input = (char) syscall_uart_read();
        }
        syscall_sleep_thread();
    }
}