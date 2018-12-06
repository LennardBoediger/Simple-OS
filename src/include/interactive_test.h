
#ifndef BS_PRAK_USERTHREAD_H
#define BS_PRAK_USERTHREAD_H

#include "printf_lib.h"

void user_thread_active (void* stack_pointer);
void user_thread_passive (void* stack_pointer);
void interactive_test_active(char c);
void interactive_test_passive(char c);

#endif //BS_PRAK_USERTHREAD_H