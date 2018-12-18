#ifndef BS_PRAK_MMU_H
#define BS_PRAK_MMU_H

#include <stdint.h>
#define L1_TABLE_SIZE 4096
#define L1_ALIGNMENT 16384
void init_mmu();

//#define IO_REGION 0x3f000000 BIS 0x3f20000


#endif //BS_PRAK_MMU_H
