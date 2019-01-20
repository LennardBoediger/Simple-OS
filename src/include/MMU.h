#ifndef BS_PRAK_MMU_H
#define BS_PRAK_MMU_H

#include <stdint.h>

#define L1_TABLE_SIZE 4096
#define L1_ALIGNMENT 16384
#define VIRT_DATA_USERSEC 0x006
#define VIRT_BSS_USERSEC 0x00F
#define VIRT_USER_STACKS 0x07D // statt 7E weil das MB 7D (und alle Bytes darüber) beschrieben werden
#define MAX_PROCESSES 8

#define DACR_CLIENT 1       //set dacr to client
#define TTBCR_USE_TTBR0 0
#define MAX_ADDR 0x08000000 //128. MB

#define INIT_KERNELSEC 0x001
#define TEXT_KERNELSEC 0x002
#define DATA_KERNELSEC 0x003
#define TEXT_USERSEC 0x004
#define RODATA_USERSEC 0x005


#define IO_PHYS_0 0x03F0
#define IO_VIRT_0 0x7E0
#define IO_PHYS_1 0x3F1
#define IO_VIRT_1 0x7E1
#define IO_PHYS_2 0x3F2
#define IO_VIRT_2 0x7E2
#define EXCEPTION_STACKS 0x07F      //127.MB


#define MMU_EN_POS 0
#define ALIGNMENT_CHECK_EN_POS 1
#define I_CHACHE_EN_POS 12
#define C_CACHE_EN_POS 2

#define EN_SECTION 1
#define XN_POS 4
#define PXN_POS 0
#define AP0_POS 10
#define AP1_POS 11
#define AP2_POS 15


void init_mmu();
void section_fullAccess(uint32_t L1_index);
void set_execNever(uint32_t L1_index);
uint32_t get_phys_user_stacks(int32_t i);
uint32_t get_L1_entry(uint32_t index);

//#define IO_REGION 0x3f000000 BIS 0x3f20000


#endif //BS_PRAK_MMU_H
