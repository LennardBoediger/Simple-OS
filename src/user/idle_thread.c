#include <stdint-gcc.h>
#include "include/uprintf.h"

//static uint32_t test;

void idle_thread(void* voidPointerOfFame){
    uprintfln("Starte den idle thread...");
//    test = 0;
//    test++;
//    uprintfln("Booting done!\n\r");
    while (1) {

    }
    /* so lange, bis interrupt -> neuer thread*/
}
