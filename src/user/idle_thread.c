#include "include/uprintf.h"

void idle_thread(void* voidPointerOfFame){
    uprintfln("Starte den idle thread...");
    uprintfln("Booting done!\n\r");
    while (1) {

    }
    /* so lange, bis interrupt -> neuer thread*/
}
