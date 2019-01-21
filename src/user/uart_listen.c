#include <stdint.h>
#include "syscalls/syscalls.h"
#include "../include/printf_lib.h"
#include "include/uprintf.h"
#include "include/interactive_test.h"


void prepare_user_process(char input){
    static void(* user_process_Ptr)(void*);
    char c = input;
    user_process_Ptr = &user_process;
    syscall_prepare_process(user_process_Ptr, (void*) &c, sizeof(c));
}

void prepare_user_thread(char input, int8_t thread_id){
    static void(* user_thread_Ptr)(void*);
    int8_t data[] = {(int8_t) input, thread_id};
    user_thread_Ptr = &user_thread;
    syscall_prepare_thread(user_thread_Ptr, (void*) &data, sizeof(input)+sizeof(thread_id));
}

void uart_listen(){
    while(1) {
        char input = (char) syscall_uart_read();
        while (input != 0) {
            prepare_user_process(input);
            input = (char) syscall_uart_read();
        }
        syscall_sleep_thread();
    }
}