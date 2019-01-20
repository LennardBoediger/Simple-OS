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
//        uprintfln("%c:%x (__:%x", temp_c, global_counter, i);
        uprintfln("%c", temp_c);
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
    void* temp_stack_pointer;
    for (temp_stack_pointer = stack_pointer; temp_stack_pointer < stack_pointer+30; temp_stack_pointer++) {
        uprintfln("USER_PROCESS() -> STACK_POINTER = %x, DATEN = %c", temp_stack_pointer,
                  *((char *) temp_stack_pointer));
    }
    user_thread(temp_stack_pointer);

    uprintfln("USER_PROCESS() -> VOR ZUGRIFF AUF GLOBAL COUNTER");
    global_counter = 0;
    uprintfln("USER_PROCESS() -> NACH ZUGRIFF AUF GLOBAL COUNTER");
    uint8_t thread_id = 1;
//    char input = *((char*) stack_pointer);
//    prepare_user_thread(input, thread_id + (uint8_t) 1);
//    prepare_user_thread(input, thread_id + (uint8_t) 2);
}
