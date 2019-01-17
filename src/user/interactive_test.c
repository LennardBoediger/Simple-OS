#include <stddef.h>
#include "include/interactive_test.h"
#include "include/interactive_test_helper.h"
#include "include/uprintf.h"
#include "syscalls/syscalls.h"

static uint32_t global_counter;

void interactive_test(char c){
    int i;
    char temp_c = c;
    global_counter++;
    for (i = 0; i < 16; i++) {
        uprintf("%c:%x (__:%x", temp_c, global_counter, i);
        syscall_sleep_thread();
    }
}


void user_thread(void* stack_pointer) {
    char input = *((char*) stack_pointer);
    interactive_test(input);
    syscall_kill_thread();
    uprintf("\n\r\n\r\n\r\n\rDEAD THREADS CANNOT KPRINTF!!!!!11!!!elf!!!\n\r\n\r\n\r\n\r");
}

void user_process(void* stack_pointer) {
    uprintfln("USER_PROCESS() -> VOR ZUGRIFF AUF GLOBAL COUNTER");
    global_counter = 0;
    uprintfln("USER_PROCESS() -> NACH ZUGRIFF AUF GLOBAL COUNTER");
    uint8_t thread_id = 1;
    char input = *((char*) stack_pointer);
//    prepare_user_thread(input, thread_id + (uint8_t) 1);
//    prepare_user_thread(input, thread_id + (uint8_t) 2);
    user_thread(stack_pointer);
}
