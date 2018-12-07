#include <stdint.h>
#include "../include/syscalls.h"
#include "../include/printf_lib.h"
#include "include/uprintf.h"
#include "include/interactive_test.h"

void(* user_thread_Ptr)(void*);



void prepare_user_thread_active(char input){
    char c = input;
    user_thread_Ptr = &user_thread_active;
    syscall_prepare_thread(user_thread_Ptr, (void*) &c, sizeof(c));
}
void prepare_user_thread_passive(char input){
    char c = input;
    user_thread_Ptr = &user_thread_passive;
    syscall_prepare_thread(user_thread_Ptr, (void*) &c, sizeof(c));
}

void uart_listen(){
//    uprintfln("START UART_LISTEN()");
    while(1) {
        char input = (char) syscall_uart_read();
        while (input != 0) {
            if (input >= 65 && input <= 90) {//Input ist ein Großbuchstabe
                //func* an syscall; syscall braucht Infos über Input
                prepare_user_thread_active(input);
            } else {
                prepare_user_thread_passive(input);
            }
            input = (char) syscall_uart_read();
        }
//        uprintfln("syscall_sleep();");
        syscall_sleep_thread();
    }
}