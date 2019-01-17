#ifndef BS_PRAK_MMU_H
#define BS_PRAK_MMU_H

#include <stdint.h>
#define L1_TABLE_SIZE 4096
#define L1_ALIGNMENT 16384
#define VIRT_DATA_USERSEC 0x006
#define VIRT_BSS_USERSEC 0x00F
#define VIRT_USER_STACKS 0x07E        // auf 126mb wird zugegriffen, in 125-118 wird tatsächlich gelesen
#define MAX_PROCESSES 8


void init_mmu();
void section_fullAccess(uint32_t L1_index);
void set_execNever(uint32_t L1_index);
uint32_t get_phys_user_stacks(int32_t i);

//#define IO_REGION 0x3f000000 BIS 0x3f20000


#endif //BS_PRAK_MMU_H
