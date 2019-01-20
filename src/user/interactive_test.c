#include <stddef.h>
#include "include/interactive_test.h"
#include "include/interactive_test_helper.h"
#include "include/uprintf.h"
#include "syscalls/syscalls.h"
#include "include/uart_listen.h"

static int32_t global_counter;

void interactive_test(char c, int8_t thread_id){
    int i;
    char temp_c = c;
    for (i = 0; i < 16; i++) {
        uprintfln("%c:%i (%i:%i)", temp_c, global_counter, thread_id, i);
        global_counter++;
//        uprintfln("%c", temp_c);
        syscall_sleep_thread();
    }
}


void user_thread(void* stack_pointer) {
    char input = *((char*) stack_pointer);
    int8_t thread_id = *(((int8_t*) stack_pointer)+1);
    interactive_test(input, thread_id);
    syscall_kill_thread();
    uprintf("\n\r\n\r\n\r\n\rDEAD THREADS CANNOT KPRINTF!!!!!11!!!elf!!!\n\r\n\r\n\r\n\r");
}

void user_process(void* stack_pointer) {
/*    void* temp_stack_pointer;
    for (temp_stack_pointer = stack_pointer; temp_stack_pointer < stack_pointer+3; temp_stack_pointer++) {
        uprintfln("USER_PROCESS() -> STACK_POINTER = %x, DATEN = %c", temp_stack_pointer,
                  *((char *) temp_stack_pointer));
    }
*/
    global_counter = 0;
    uint8_t thread_id = 1;
    char input = *((char*) stack_pointer);
    int8_t input_array[] = {input, thread_id};
    void* input_pointer = (void*) input_array;
    prepare_user_thread(input, thread_id + (uint8_t) 1);
    prepare_user_thread(input, thread_id + (uint8_t) 2);
    user_thread(input_pointer);
}
