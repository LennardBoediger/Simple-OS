#include <stdint.h>
#include "../syscalls/syscalls.h"
#include "../include/printf_lib.h"
#include "include/uprintf.h"
#include "include/interactive_test.h"



void prepare_user_thread_active(char input){
    static void(* user_thread_Ptr_act)(void*);
    char c = input;
    user_thread_Ptr_act = &user_thread_active;
    syscall_prepare_thread(user_thread_Ptr_act, (void*) &c, sizeof(c));
}
void prepare_user_thread_passive(char input){
    static void(* user_thread_Ptr_pas)(void*);
    char c = input;
    user_thread_Ptr_pas = &user_thread_passive;
    syscall_prepare_thread(user_thread_Ptr_pas, (void*) &c, sizeof(c));
}

void uart_listen(){
//    uprintfln("START UART_LISTEN()");
    while(1) {
        //TODO: INTERRUPTS AUS!!
        char input = (char) syscall_uart_read();
        //BIS HIER
        while (input != 0) {
            if (input >= 'A' && input <= 'Z') {
                //func* an syscall; syscall braucht Infos über Input
                prepare_user_thread_active(input);
            } else {
                prepare_user_thread_passive(input);
            }
            //TODO: INTERRUPT AUS
            input = (char) syscall_uart_read();
            //BIS HIER
        }
//        uprintfln("syscall_sleep();");
        syscall_sleep_thread();
    }
}