#ifndef BS_PRAK_MMU_H
#define BS_PRAK_MMU_H

#include <stdint.h>
#define L1_TABLE_SIZE 4096
#define L1_ALIGNMENT 16384
#define DATA_USERSEC 0x0006
#define MAX_PROCESSES 8
const uint32_t phys_user_stacks[] = {0x07D, 0x07C, 0x07B, 0x07A, 0x079, 0x078, 0x077, 0x076};
static uint32_t L1_table[L1_TABLE_SIZE] __attribute__((aligned(L1_ALIGNMENT)));


void init_mmu();
void section_fullAccess(uint32_t L1_index);
void set_execNever(uint32_t L1_index);

//#define IO_REGION 0x3f000000 BIS 0x3f20000


#endif //BS_PRAK_MMU_H
