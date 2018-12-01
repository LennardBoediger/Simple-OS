#include <stdint.h>
#include "../include/syscalls.h"
#include "../include/kprintf.h" // todo replace
#include "../include/init_thread.h" //todo remove


void uart_listen(){
    kprintfln("START UART_LISTEN()");
    char input = (char)syscall_uart_read();
    while (input != '\0'){
        if(input >= 65 && input <= 90){//Input ist ein Großbuchstabe
            //func* an syscall; syscall braucht Infos über Input
            kprintfln("syscall_prepare_thread(AKTIVE Wait thread); mit char = %c", input);
        } else{
            kprintfln("syscall_prepare_thread(PASSIVE Wait thread); mit char = %c", input);
        }
        input = (char)syscall_uart_read();
    }
    kprintfln("syscall_sleep();");

}

void prepare_uart_listenthread () {
    void(* uart_listen_Ptr)(void*);
    uart_listen_Ptr = &uart_listen;
    prepare_thread(uart_listen_Ptr, (void*)NO_STACK_ADRESS, 0, 0);
}
