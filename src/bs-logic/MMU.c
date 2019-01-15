#include "../include/MMU-helper.h"
#include "../include/kprintf.h"
#include "../include/MMU.h"
#include "../include/process.h"
#define DACR_CLIENT 1       //set dacr to client
#define TTBCR_USE_TTBR0 0
#define MAX_ADDR 0x08000000 //128. MB

#define INIT_KERNELSEC 0x0001
#define TEXT_KERNELSEC 0x0002
#define DATA_KERNELSEC 0x0003
#define TEXT_USERSEC 0x0004
#define RODATA_USERSEC 0x0005


#define IO_PHYS_0 0x3F0
#define IO_VIRT_0 0x7E0
#define IO_PHYS_1 0x3F1
#define IO_VIRT_1 0x7E1
#define IO_PHYS_2 0x3F2
#define IO_VIRT_2 0x7E2
#define EXCEPTION_STACKS 0x07F      //127.MB

#define VIRT_USER_STACKS 0x07E        // auf 126mb wird zugegriffen, in 125-118 wird tatsächlich gelesen

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


void entry_is_section(uint32_t L1_index) {
    //ACHTUNG: HIER KEINE BITSETZUNG!!!
    if (L1_index == VIRT_USER_STACKS) {
        L1_table[L1_index] = (phys_user_stacks[get_current_process()] << 20);        // NICHT 1 zu 1 mapping
    } else {
        L1_table[L1_index] = (L1_index << 20);        // 1 zu 1 mapping
        L1_table[L1_index] |= (1 << EN_SECTION);  // SECTION ENABLED
    }
}

void entry_is_invalid(uint32_t L1_index) {
    L1_table[L1_index] &= ~(1 << EN_SECTION);
    L1_table[L1_index] &= 0;
}

void section_sys_rw(uint32_t L1_index) {
    entry_is_section(L1_index);
    L1_table[L1_index] |= (1 << AP0_POS);
    L1_table[L1_index] &= ~(1 << AP1_POS);
    L1_table[L1_index] &= ~(1 << AP2_POS);
}

void section_sys_r(uint32_t L1_index) {
    entry_is_section(L1_index);
    L1_table[L1_index] |= (1 << AP0_POS);
    L1_table[L1_index] &= ~(1 << AP1_POS);
    L1_table[L1_index] |= (1 << AP2_POS);
}

void section_usr_r(uint32_t L1_index) {
    entry_is_section(L1_index);
    L1_table[L1_index] &= ~(1 << AP0_POS);
    L1_table[L1_index] |= (1 << AP1_POS);
    L1_table[L1_index] &= ~(1 << AP2_POS);
}

void section_fullAccess(uint32_t L1_index) {
    entry_is_section(L1_index);
    L1_table[L1_index] |= (1 << AP0_POS);
    L1_table[L1_index] |= (1 << AP1_POS);
    L1_table[L1_index] &= ~(1 << AP2_POS);
}

void set_execNever(uint32_t L1_index) {
    L1_table[L1_index] |= (1 << XN_POS);
}

void set_privExecNever(uint32_t L1_index) {
    L1_table[L1_index] |= (1 << PXN_POS);
}

uint32_t addr_to_index(uint32_t* addr) {
    return (((uint32_t) addr) >> 20);
}


void set_L1(){
    uint32_t i;
    for(i = 0; i < L1_TABLE_SIZE; i++) {
        entry_is_invalid(i);
    }
    section_sys_rw(IO_PHYS_0);
    set_execNever(IO_PHYS_0);
    kprintfln("SET_L1 -> L1[IO_PHYS_0] = %x", L1_table[IO_PHYS_0]);
    section_sys_rw(IO_PHYS_1);
    set_execNever(IO_PHYS_1);
    kprintfln("SET_L1 -> L1[IO_PHYS_1] = %x", L1_table[IO_PHYS_1]);
    section_sys_rw(IO_PHYS_2);
    set_execNever(IO_PHYS_2);
    kprintfln("SET_L1 -> L1[IO_PHYS_2] = %x", L1_table[IO_PHYS_2]);
    section_sys_r(INIT_KERNELSEC);
    kprintfln("SET_L1 -> L1[INIT_KERNELSEC] = %x", L1_table[INIT_KERNELSEC]);
    section_sys_r(TEXT_KERNELSEC);
    kprintfln("SET_L1 -> L1[TEXT_KERNELSEC] = %x", L1_table[TEXT_KERNELSEC]);
    section_sys_rw(DATA_KERNELSEC);
    set_execNever(DATA_KERNELSEC);
    kprintfln("SET_L1 -> L1[DATA_KERNELSEC] = %x", L1_table[DATA_KERNELSEC]);
    section_usr_r(TEXT_USERSEC);
    set_privExecNever(TEXT_USERSEC);
    kprintfln("SET_L1 -> L1[DATA_KERNELSEC] = %x", L1_table[TEXT_USERSEC]);
    //TODO nur usr_r
    section_fullAccess(RODATA_USERSEC);
    set_execNever(RODATA_USERSEC);
    kprintfln("SET_L1 -> L1[DATA_USERSEC]  = %x", L1_table[RODATA_USERSEC]);

    uint32_t process;
    for(process = 0; process < MAX_PROCESSES; process++) {
        section_fullAccess(DATA_USERSEC+process);
        set_execNever(DATA_USERSEC+process);
        kprintfln("SET_L1 -> L1[DATA_USERSEC]  = %x", L1_table[DATA_USERSEC+process]);
    }
    section_sys_rw(EXCEPTION_STACKS);
    set_execNever(EXCEPTION_STACKS);
    kprintfln("SET_L1 -> L1[EXCEPTION_STACKS]  = %x", L1_table[EXCEPTION_STACKS]);
}


//CCacheEn(2) = 0, ICacheEn(12) = 0, AlignmentCheckEnable(1) = 1, MmuEnable(0) = 1
void init_sctlr() {
    uint32_t sctlr = get_sctlr();
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
    set_L1();
    init_sctlr();
    kprintfln("INIT MMU DONE!");
}