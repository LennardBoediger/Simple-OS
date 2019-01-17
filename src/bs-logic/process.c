#include <stdint.h>
#include "../include/kprintf.h"
#include "../include/MMU.h"
#include "../include/systemfunctions.h"

#define USRDATA_USRBSS_SIZE 0x00100000

int32_t current_process;
int32_t unborn_process;


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
    }
}

void copy_user_stuff(){

}
// kopiert Daten aus der virtuellen data und bss(alles0) sektion in die section von unborn (virtuelle adresse + unborn+1)
// dafür müssen die pysikalischen adressen 1 zu 1 gemappt sein mit der brechentigung nur kernel rw
// die virtuelle muss auf die jeweilige pysikalische zeigen aber bei swap process nicht bei new process
void new_process() {
    //TODO ARRAY-SUCHE
    unborn_process += 1;

    //kopiert die Daten aus dem virtuellen (zeigend auf aktuellen) Prozess in den Speicher des neuen physikalischen Prozesses
    memcopy((void*)(VIRT_DATA_USERSEC<<20), (void*)((VIRT_DATA_USERSEC+unborn_process)<<20), USRDATA_USRBSS_SIZE);
    mymalloc((void*)((VIRT_BSS_USERSEC+unborn_process)<<20), USRDATA_USRBSS_SIZE);
}

void init_process(){
    current_process = 0;
    unborn_process = 0;
    //copy data und text
    //bss = 0
}
