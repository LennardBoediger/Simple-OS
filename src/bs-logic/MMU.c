#include "../include/MMU-helper.h"
#include "../include/kprintf.h"
#include "../include/MMU.h"
#include "../include/process.h"


const uint32_t phys_user_stacks[] = {0x05D, 0x05C, 0x05B, 0x05A, 0x059, 0x058, 0x057, 0x056}; //{0x07D, 0x07C, 0x07B, 0x07A, 0x079, 0x078, 0x077, 0x076};
static uint32_t L1_table[L1_TABLE_SIZE] __attribute__((aligned(L1_ALIGNMENT)));

uint32_t get_L1_entry(uint32_t index) {
    return L1_table[index];
}

uint32_t get_phys_user_stacks(int32_t i) {
    if (i > 7) {
        kprintfln("GET_PHYS_USER_STACKS() -> INDEX ZU GROß!!!");
    }
    return phys_user_stacks[i];
}

//TODO: initialer Fall funktioniert (wahrscheinlich). Wie ist es beim Process-Switch??
// WEGEN BERECHTIGUNGEN NIE DIREKT AUFRUFEN
void entry_is_section(uint32_t L1_index) {
    //ACHTUNG: HIER KEINE BITSETZUNG!!!
    switch (L1_index) {
        case VIRT_USER_STACKS:
            L1_table[L1_index] = (phys_user_stacks[get_current_process()] << 20);
            break;
        case VIRT_BSS_USERSEC:
            //die physikalischen Adressen liegen in den MBs über der virtuellen (s. weiter unten)
            L1_table[L1_index] = ((VIRT_BSS_USERSEC + (uint32_t) get_current_process()) << 20);
            break;
        case VIRT_DATA_USERSEC:
            L1_table[L1_index] = ((VIRT_DATA_USERSEC + (uint32_t) get_current_process()) << 20);
            break;
        default:
            L1_table[L1_index] = (L1_index << 20);        // 1 zu 1 mapping
    }
    L1_table[L1_index] |= (1 << EN_SECTION);  // SECTION ENABLED
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
    kprintfln("SET_L1 -> L1[TEXT_USERSEC] = %x", L1_table[TEXT_USERSEC]);
    //TODO nur usr_r
    section_fullAccess(RODATA_USERSEC);
    set_execNever(RODATA_USERSEC);
    kprintfln("SET_L1 -> L1[RODATA_USERSEC]  = %x", L1_table[RODATA_USERSEC]);

    //KOPIEREN EINMALIG DATEN AUS VIRTUELLEN ADRESSE IN PHYSIKALISCHE ADRESSE DES 0. PROZESSES
    //setzt VIRT_USER_STACKS, VIRT_BSS_USERSEC und VIRT_DATA_USERSEC auf den nullten Prozess (mit full-access)
    section_fullAccess(VIRT_USER_STACKS);
    set_execNever(VIRT_USER_STACKS);
    kprintfln("SET_L1 -> L1[VIRT_USER_STACKS]  = %x", L1_table[VIRT_USER_STACKS]);
    section_fullAccess(VIRT_BSS_USERSEC);
    set_execNever(VIRT_BSS_USERSEC);
    kprintfln("SET_L1 -> L1[VIRT_USER_STACKS]  = %x", L1_table[VIRT_USER_STACKS]);
    section_fullAccess(VIRT_DATA_USERSEC);
    set_execNever(VIRT_DATA_USERSEC);
    kprintfln("SET_L1 -> L1[VIRT_DATA_USERSEC]  = %x", L1_table[VIRT_DATA_USERSEC]);


    //TODO DONE?! physikalisch usrSTACKS, usrBSS, usrDATA 1-zu-1 auf kernel-rw
    uint32_t process;
    kprintfln("PROZESSE:");
    for(process = 1; process < MAX_PROCESSES; process++) {
        section_sys_rw(VIRT_DATA_USERSEC + process);
        section_sys_rw(VIRT_BSS_USERSEC + process);
        //TODO Müssen die wirklich 1-1 gemappt werden?
        // TODO WIEDER ZURÜCK AUF sys_rw
        section_sys_rw(get_phys_user_stacks(process-1));

        set_execNever(VIRT_DATA_USERSEC + process);
        set_execNever(VIRT_BSS_USERSEC + process);
        //TODO Müssen die wirklich 1-1 gemappt werden?
        set_execNever(get_phys_user_stacks(process-1));

        kprintfln("SET_L1 -> L1[VIRT_DATA_USERSEC + %i]  = %x", process, L1_table[VIRT_DATA_USERSEC+process]);
        kprintfln("SET_L1 -> L1[VIRT_BSS_USERSEC + %i]  = %x", process, L1_table[VIRT_BSS_USERSEC+process]);
//        kprintfln("SET_L1 -> L1[USER_STACK + %i]  = %x", process, L1_table[get_phys_user_stacks(process-1)]);
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