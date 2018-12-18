#ifndef BS_PRAK_MMU_HELPER_H
#define BS_PRAK_MMU_HELPER_H
#include <stdint.h>

void init_dacr(uint32_t dacr_value);
void init_ttbcr(uint32_t ttbcr_value);
void init_ttbr0(uint32_t* table_pointer);
void init_sctlr();
uint32_t get_sctlr();
void set_sctlr(uint32_t sctlr);

#endif //BS_PRAK_MMU_HELPER_H
