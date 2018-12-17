#include "../include/MMU-helper.h"
#include "../include/kprintf.h"
#define DACR_CLIENT 1       //set dacr to client
#define TTBCR_USE_TTBR0 0
#define L1_TABLE_SIZE 4096
#define L1_ALLIGNMENT 16384
#define MAX_ADDR 0x08000000 //128. MB
#define FAULT_VALUE 0xfffffffc
//#define IO_REGION 0x3f000000 BIS 0x3f20000

static uint32_t L1_table[L1_TABLE_SIZE]
__attribute__((alligned(L1_ALLIGNMENT)));

uint32_t addr_to_index(uint32_t* addr) {
    return (((uint32_t) addr) >> 20);
}

//TODO BITS BEACHTEN
void set_L1(){
    uint32_t i;
    for(i = 0; i < max_addr_i-1; i++) {
        L1_table[i] = (i << 20);
        //setzt das 0. und 1. Bit auf Null, da Werte bei Init alle invalide
        L1_table[i] = L1_table[i] & FAULT_VALUE;
    }

    uint32_t max_addr_i = addr_to_index((uint32_t*) MAX_ADDR);
    for(i; i < L1_TABLE_SIZE; i++) {

        if (i > 0 && i < max_addr_i - 1) {  //user  kernel und stacks breich
            L1_table[i] = (i << 20);       //1 ZU 1 MAPPING
        } else if (i == addr_to_index((uint32_t *)IO_REGION)) { // IO BEREICH

        }
    }
}

void init_L1_table(){
    set_L1();
}

void init_sctlr() {
    uint32_t sctlr = get_sctlr();
    CCacheEn(2) = 0, ICacheEn(12) = 0, AlignmentCheckEnable(1) = 1, MmuEnable(0) = 1
    set_sctlr(sctlr);
}

void init_mmu() {
    init_dacr(DACR_CLIENT);
    init_ttbcr(TTBCR_USE_TTBR0);
    init_ttbr0(L1_table);
    init_L1_table();

    //TODO: WAS SOLL BEIM controlReg GESETZT WERDEN??? (Skriptseite: 1707ff)
    //TODO: AccessFlagEnable(29) = 0?, TexRemapEnable(28) = 0?, CacheEnable(2) = 0?, AlignmentCheckEnable(1) = 1!, MmuEnable(0) = 1!
    init_sctlr();

    //TODO: not_1_to_1_mapping

    kprintfln("INIT DONE!!");
}