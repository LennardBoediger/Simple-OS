#include <stdint.h>
#include "../include/kprintf.h"
#include "../include/MMU.h"
#include "../include/systemfunctions.h"

int32_t current_process;
int32_t unborn_process;
/*extern char VAR1[];
extern char VAR2[];
unsigned int usrbss_begin = (unsigned int) VAR1;
unsigned int usrbss_end = (unsigned int) VAR2;
*/
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
        section_fullAccess(0x07E); //todo lesbarer 0x07E = virtual user stacks
        //TODO kann wahrscheinlich raus, da redundant
        set_execNever(0x07E);
    }
}

void copy_user_stuff(){

}

void new_process() {
//    uint32_t data_size = usrbss_begin - (DATA_USERSEC<<20);
    //TODO ARRAY-SUCHE
    unborn_process += 1;
//    memcopy((void*)(DATA_USERSEC<<20), (void*)((DATA_USERSEC+unborn_process)<<20), data_size);
//    mymalloc((void*)(((DATA_USERSEC+unborn_process)<<20) + data_size), usrbss_end-usrbss_begin);
}

void init_process(){
    current_process = 0;
    unborn_process = 0;
    //copy data und text
    //bss = 0
}
