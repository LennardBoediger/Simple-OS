#include "../include/MMU-helper.h"
#include "../include/kprintf.h"
#include "../include/MMU.h"
#define DACR_CLIENT 1       //set dacr to client
#define TTBCR_USE_TTBR0 0
#define MAX_ADDR 0x08000000 //128. MB
#define FAULT_VALUE 0xfffffffc

#define INIT_KERNELSEC 1
#define TEXT_KERNELSEC 2
//#define RODATA_KSEC 5
#define DATA_KERNELSEC 3
//#define BSS_USEC 7
#define TEXT_USERSEC 4
#define DATA_USERSEC 5
//TODO: IO-Bereiche & Stacks

#define MMU_EN_POS 0
#define ALIGNMENT_CHECK_EN_POS 1
#define I_CHACHE_EN_POS 12
#define C_CACHE_EN_POS 2

/*
 *
 *
 */
#define ALLWAYS_ONE_POS 1
#define XN_POS 4
#define PXN_POS 0
#define AP0_POS 10
#define AP1_POS 11
#define AP2_POS 15

static uint32_t L1_table[L1_TABLE_SIZE] __attribute__((aligned(L1_ALIGNMENT)));

void entry_is_section(uint32_t L1_index) {
    //ACHTUNG: HIER KEINE BITSETZUNG!!!
    L1_table[L1_index] = (L1_index << 20);        // 1 zu 1 mapping
    L1_table[L1_index] |= (1 << ALLWAYS_ONE_POS);  // SECTION ENABLED
}

//TODO verständlicher
void entry_is_invalid(uint32_t L1_index) {
    L1_table[L1_index] &= ~(1 << ALLWAYS_ONE_POS);  // SECTION DISABLED
    L1_table[L1_index] &= 0;
}

void section_sys_rw(uint32_t L1_index) {
    entry_is_section(L1_index);
    L1_table[L1_index] |= (1 << AP0_POS);  // AP = 011 Voller zugriff von allen (zum testen)
    L1_table[L1_index] &= ~(1 << AP1_POS);
    L1_table[L1_index] &= ~(1 << AP2_POS);
}

void section_sys_r(uint32_t L1_index) {
    entry_is_section(L1_index);
    L1_table[L1_index] |= (1 << AP0_POS);  // AP = 011 Voller zugriff von allen (zum testen)
    L1_table[L1_index] &= ~(1 << AP1_POS);
    L1_table[L1_index] |= (1 << AP2_POS);
}

void section_usr_r(uint32_t L1_index) {
    entry_is_section(L1_index);
    L1_table[L1_index] &= ~(1 << AP0_POS);  // AP = 011 Voller zugriff von allen (zum testen)
    L1_table[L1_index] |= (1 << AP1_POS);
    L1_table[L1_index] &= ~(1 << AP2_POS);
}

void section_fullAccess(uint32_t L1_index) {
    entry_is_section(L1_index);
    L1_table[L1_index] |= (1 << AP0_POS);  // AP = 011 Voller zugriff von allen (zum testen)
    L1_table[L1_index] |= (1 << AP1_POS);
    L1_table[L1_index] &= ~(1 << AP2_POS);
}

void set_execNever(uint32_t L1_index) {
    L1_table[L1_index] |= (1 << XN_POS);
}

void set_privExecNever(uint32_t L1_index) {
    L1_table[L1_index] |= (1 << PXN_POS);
}
/*
 *
 *
 */

uint32_t addr_to_index(uint32_t* addr) {
    return (((uint32_t) addr) >> 20);
}

void set_L1(){
    uint32_t i;
    uint32_t max_addr_i = addr_to_index((uint32_t*) MAX_ADDR);
    for(i = 0; i < L1_TABLE_SIZE; i++) {
        section_fullAccess(i);          //TODO: müssen als INVALID markiert werden
    }
    section_fullAccess(0);
//    set_execNever(0); //TODO: FRAGEN, was das drinne liegt...
    kprintfln("SET_L1 -> L1[0] = %x", L1_table[0]);

    section_fullAccess(INIT_KERNELSEC);  //TODO ...
    kprintfln("SET_L1 -> L1[INIT_KERNELSEC] = %x", L1_table[INIT_KERNELSEC]);

    section_sys_rw(TEXT_KERNELSEC); //TODO rw?
//    set_privExecNever(TEXT_KERNELSEC); NUR TESTWEISE!
    kprintfln("SET_L1 -> L1[TEXT_KERNELSEC] = %x", L1_table[TEXT_KERNELSEC]);

    section_sys_rw(DATA_KERNELSEC);
    set_execNever(DATA_KERNELSEC);
    kprintfln("SET_L1 -> L1[DATA_KERNELSEC] = %x", L1_table[DATA_KERNELSEC]);

    //TODO: DIE BEIDEN KPRINTFs PRINTEN KOMISCHE SACHEN
    section_usr_r(TEXT_USERSEC);
    set_privExecNever(TEXT_USERSEC);
    kprintfln("SET_L1 -> L1[TEXT_USERSEC] = %x", L1_table[TEXT_USERSEC]);

    section_fullAccess(DATA_USERSEC);
    set_execNever(DATA_USERSEC);
    kprintfln("SET_L1 -> L1[DATA_USERSEC] = %x", L1_table[DATA_USERSEC]);
    //TODO IO-BEREICH und
    // TODO: Weiter eingrenzen
}

void init_L1_table(){
    set_L1();
    //
}

void init_sctlr() {
    uint32_t sctlr = get_sctlr();
    //CCacheEn(2) = 0, ICacheEn(12) = 0, AlignmentCheckEnable(1) = 1, MmuEnable(0) = 1
    sctlr &= ~(1 << C_CACHE_EN_POS);            //C_Cache aus
    sctlr &= ~(1 << I_CHACHE_EN_POS);           //I_Cache aus
    sctlr |= (1 << ALIGNMENT_CHECK_EN_POS);     //ALignment an
    sctlr |= (1 << MMU_EN_POS);                 //MMU an
    set_sctlr(sctlr);
    flush_tlb();
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

    kprintfln("INIT MMU DONE!!");
}