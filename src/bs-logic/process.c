#include <stdint.h>
#include "../include/kprintf.h"
#include "../include/MMU.h"
#include "../include/MMU-helper.h"
#include "../include/systemfunctions.h"

#define USRDATA_USRBSS_SIZE 0x100000
#define PROCESS_IN_USE 1
#define PROCESS_NOT_IN_USE 0

int32_t current_process;
int32_t unborn_process;

uint32_t prozesse[8];

int32_t get_current_process(){
    return current_process;
}

int32_t get_unborn_process(){
    return unborn_process;
}

void swap_process(int32_t next_process) {
    if (next_process != current_process) {
        //old section_fullAccess(get_phys_user_stacks(next_process));
        current_process = next_process;
        //VIRTUELL STACKS, BSS, DATA auf PHYSICAL current-process
        //macht auch den wichtigen remap
        section_fullAccess(VIRT_DATA_USERSEC);
        set_execNever(VIRT_DATA_USERSEC);
        section_fullAccess(VIRT_BSS_USERSEC);
        set_execNever(VIRT_BSS_USERSEC);
        section_fullAccess(VIRT_USER_STACKS);
        set_execNever(VIRT_USER_STACKS);
        flush_tlb();
    }
}

int32_t find_free_process() {
    uint32_t i;
    for (i = 0; i < MAX_PROCESSES; i++) {
        if (prozesse[i] == PROCESS_NOT_IN_USE) {
            prozesse[i] = PROCESS_IN_USE;
            unborn_process = i;
            return 0;
        }
    }
    // Wenn kein freier Prozess gefunden wurde
    return -1;
}

void kill_process(int32_t process) {
    prozesse[process] = PROCESS_NOT_IN_USE;
}

// kopiert Daten aus der virtuellen data und bss(alles0) sektion in die section von unborn (virtuelle adresse + unborn+1)
// dafür müssen die pysikalischen adressen 1 zu 1 gemappt sein mit der brechentigung nur kernel rw
int32_t new_process() {
    if (find_free_process() == -1) {
        kprintfln("K1 freier Prozess");
        return -1;
    }
    kprintfln("NEW_PROCESS() -> unborn_process = %i ", unborn_process);
    //kopiert die Daten aus dem virtuellen (zeigend auf aktuellen) Prozess in den Speicher des neuen physikalischen Prozesses
    memcopy((void*)(VIRT_DATA_USERSEC<<20), (void*)((VIRT_DATA_USERSEC+unborn_process)*0x100000), USRDATA_USRBSS_SIZE);
    mymalloc((void*)((VIRT_BSS_USERSEC+unborn_process)*0x100000), USRDATA_USRBSS_SIZE);
    // TODO STACK MUSS GEMALLOCT WERDEN
    kprintfln("NEW_PROCESS -> INIT DONE");
    return 0;
}

//setzte alle PROZESSE
void init_process(){
    int32_t i;
    for (i = 0; i < MAX_PROCESSES; i++) {
        prozesse[i] = PROCESS_NOT_IN_USE;
    }
    current_process = 0;
    unborn_process = 0;
    prozesse[current_process] = PROCESS_IN_USE;
}
