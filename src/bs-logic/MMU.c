#include "../include/MMU-helper.h"
#include "../include/kprintf.h"
#define DACR_CLIENT 1       //set dacr to client
#define TTBCR_USE_TTBR0 0
#define L1_TABLE_SIZE 4096
#define L1_ALLIGNMENT 16384
#define MAX_ADDR 0x08000000 //128. MB
#define FAULT_VALUE 0xfffffffc

#define AP0_POS 10
#define AP1_POS 11
#define AP2_POS 12

#define MMU_EN_POS 0
#define ALIGNMENT_CHECK_EN_POS 1
#define I_CHACHE_EN_POS 12
#define C_CACHE_EN_POS 2


//#define IO_REGION 0x3f000000 BIS 0x3f20000

static uint32_t L1_table[L1_TABLE_SIZE]
__attribute__((alligned(L1_ALLIGNMENT)));

uint32_t addr_to_index(uint32_t* addr) {
    return (((uint32_t) addr) >> 20);
}

//TODO BITS BEACHTEN
void set_L1(){
    uint32_t i;
    uint32_t max_addr_i = addr_to_index((uint32_t*) MAX_ADDR);
    for(i = 0; i < L1_TABLE_SIZE; i++) {
        L1_table[i] = (i << 20);        // 1 zu 1 mapping
        L1_table[i] |= (1 << AP0_POS);  // AP = 011 Voller zugriff von allen (zum testen)
        L1_table[i] |= (1 << AP1_POS);
        L1_table[i] &= ~(1 << AP2_POS);

 // TODO: Weiter eingrenzen
  /* if (i > 0 && i < max_addr_i - 1) {                             //user  kernel und stacks breich
            L1_table[i] = (i << 20);       //1 ZU 1 MAPPING
        } else if (i == addr_to_index((uint32_t *)IO_REGION)) {     // IO BEREICH
        }
        else{                                                       //Out of boud
            L1_table[i] = L1_table[i] & FAULT_VALUE;
        }
*/
    }
    kprintfln("L1_table[1]=00000000001 bit 15 = 0 bit 11-10=11 %x",L1_table[1]);
}

void init_L1_table(){
    set_L1();
}

void init_sctlr() {
    uint32_t sctlr = get_sctlr();
    //CCacheEn(2) = 0, ICacheEn(12) = 0, AlignmentCheckEnable(1) = 1, MmuEnable(0) = 1
    sctlr &= ~(1 << C_CACHE_EN_POS);            //C_Cache aus
    sctlr &= ~(1 << I_CHACHE_EN_POS);           //I_Cache aus
    sctlr |= (1 << ALIGNMENT_CHECK_EN_POS);     //ALignment an
    sctlr |= (1 << MMU_EN_POS);                 //MMU an
    set_sctlr(sctlr);
}

void init_mmu() {
    init_dacr(DACR_CLIENT);
    init_ttbcr(TTBCR_USE_TTBR0);
    init_ttbr0(L1_table);
    init_L1_table();

    //TODO: WAS SOLL BEIM controlReg GESETZT WERDEN??? (Skriptseite: 1707ff)
   // TODO:/*nö AccessFlagEnable(29) = 0?, TexRemapEnable(28) = 0?,*/
    // CacheEnable(2) = 0?, AlignmentCheckEnable(1) = 1! optional ,
    // MmuEnable(0) = 1! Ichach bit (12 I)

    init_sctlr();

    //TODO: not_1_to_1_mapping

    kprintfln("INIT DONE!!");
}