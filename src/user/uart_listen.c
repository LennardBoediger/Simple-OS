#include <stdint.h>
#include "../include/syscalls.h"
#include "../include/printf_lib.h"
#include "include/uprintf.h"
#include "../include/init_thread.h" //todo remove
#include "../include/interactive_test.h"

char global_char;
void(* user_thread_Ptr)(void*);



void prepare_user_thread(char input){
    //TODO: testen, ob global notwendig
    global_char = input;
    user_thread_Ptr = &user_thread;
    syscall_prepare_thread(user_thread_Ptr, (void*) &global_char, sizeof(global_char), 0);
}

void uart_listen(){
    uprintfln("START UART_LISTEN()");
    while(1) {
        char input = (char) syscall_uart_read();
        //TODO vermutlich doch '\0', eigentlich (c99): NULL
        while (input != 0) {
            if (input >= 65 && input <= 90) {//Input ist ein Großbuchstabe
                //func* an syscall; syscall braucht Infos über Input
                uprintfln("syscall_prepare_thread(AKTIVE Wait thread); mit char = %c", input);
                //prepare_user_thread(input);
            } else {
                uprintfln("syscall_prepare_thread(PASSIVE Wait thread); mit char = %c", input);
                prepare_user_thread(input);
            }
            input = (char) syscall_uart_read();
        }
        uprintfln("syscall_sleep();");
        syscall_sleep_thread();
    }
}







//TODO USER von SYS trennen
void prepare_uart_listenthread () {
    void(* uart_listen_Ptr);
    uart_listen_Ptr = &uart_listen;
    prepare_thread(uart_listen_Ptr, (void*)NO_STACK_ADRESS, 0, 0);
}

