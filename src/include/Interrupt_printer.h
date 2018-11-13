#include <stdint.h>


void print_interrupt(uint32_t stackadress, uint32_t cpsr, uint32_t spsr, char* interrupt_name,
                     int32_t pc_offset, int8_t is_data_ab);
void print_psr_mode(uint32_t _psr);
void print_timerval(uint32_t cpsr);
void print_data_abort_reason();