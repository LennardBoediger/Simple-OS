#ifndef USER_ASM_H
#define USER_ASM_H

#include <stdint.h>

void branchto(uint32_t* branch_des);
void branch_to_np();
void read_address(uint32_t* address);
void write_address(uint32_t* address);
void stack_overflow(uint32_t data);

#endif