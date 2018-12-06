#include "../include/printf_lib.h"
#include "../include/syscall_helper.h"

/** SYSCALLS
 *  SWI_NO  RÜCKGABE  PARAMETER
 *
 *  swi 0   void      ()
 *      --> aktuellen Thread beenden
 *
 *  swi 1   void      (void Funktions-Pointer, void* zusätzl. Daten, uint32_t Datengröße, uint8_t force_idle)
 *      --> neuen Thread erzeugen
 *
 *  swi 2   void      ()
 *      --> aktuellen Thread verzögern
 *
 *  swi 3   void      (char to_send)
 *      --> einzelnes Char an UART senden
 *
 *  swi 4   uint32_t  ()
 *      --> einzelnes Char von UART lesen
 */
void syscall_kill_thread(){
    kprintfln("SYSCALL_KILL_THREAD was called!!");
    asm("swi 0"); // swi SYS_KILL_THREAD
}
void syscall_prepare_thread(void (*pc)(void*), void* irq_stack_data, uint32_t irq_stack_data_size, uint8_t force_idle){
    asm("swi 1"); //swi SYS_PREPARE_THREAD
}

void syscall_sleep_thread() {
    asm("swi 2");
}

void syscall_uart_print(char to_send) {
    write_r8_char(to_send);
    asm("swi 3");
}

uint32_t syscall_uart_read() {
    asm("swi 4");
    //im Userfunktionsaufruf syscall_uart_read: r8 in eine Variable laden
    return read_r8_char();
}