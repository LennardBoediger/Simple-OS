#include <stddef.h>
#include "include/interactive_test.h"
#include "include/interactive_test_helper.h"
#include "include/uprintf.h"
#include "../syscalls/syscalls.h"

uint32_t global_counter;


void interactive_test(char c, uint8_t thread_id){
    int i;
    char temp_c = c;
    uint8_t temp_id = thread_id;
    global_counter++;
    for (i = 1; i < 16; i++) {
        uprintfln("%c:%x (%x:%x)", temp_c, global_counter, temp_id, i);
        syscall_sleep_thread();
    }
}

void prepare_user_thread(char input, uint8_t thread_id){
    static void(* user_thread_Ptr)(void*);
   // char c = input;
    uint8_t thread_data[2];
    thread_data[0] = (uint8_t)input;
    thread_data[1] = thread_id;
    user_thread_Ptr = &user_thread;
    syscall_prepare_thread(user_thread_Ptr, (void*) &thread_data, sizeof(thread_data));
}

void user_thread(void* stack_pointer) {
    char input = *((uint8_t*) stack_pointer);
    uint8_t thread_id = *((uint8_t*) stack_pointer-1);
    interactive_test(input, thread_id);
    syscall_kill_thread();

    uprintf("\n\r\n\r\n\r\n\rDEAD THREADS CANNOT KPRINTF!!!!!11!!!elf!!!\n\r\n\r\n\r\n\r");
}

void user_process(void* stack_pointer) {
    global_counter = 0;
    uint8_t thread_id = 1;
    char input = *((char*) stack_pointer);
    prepare_user_thread(input, thread_id + (uint8_t) 1);
    prepare_user_thread(input, thread_id + (uint8_t) 2);
    user_thread(stack_pointer);
}
