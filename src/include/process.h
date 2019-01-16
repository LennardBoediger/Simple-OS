//
// Created by pablo on 15.01.19.
//

#ifndef BS_PRAK_PROCESS_H
#define BS_PRAK_PROCESS_H

#include <stdint.h>

int32_t get_current_process();
int32_t get_unborn_process();
void swap_process(int32_t next_process);
uint32_t new_process();
void init_process();
void print_current_process_state();

#endif //BS_PRAK_PROCESS_H
