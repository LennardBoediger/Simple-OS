
#ifndef BS_PRAK_USERTHREAD_H
#define BS_PRAK_USERTHREAD_H

#include "../../include/printf_lib.h"

void user_thread (void* stack_pointer);
void interactive_test(char c, int8_t thread_id);
void user_process(void* stack_pointer);

#endif //BS_PRAK_USERTHREAD_H