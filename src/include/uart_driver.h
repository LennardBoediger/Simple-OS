#ifndef BS_PRAK_UART_DRIVER_H
#define BS_PRAK_UART_DRIVER_H
#include <stdint.h>

struct uart {
    uint32_t DR;
    uint32_t unused[5];
    uint32_t FR;
    uint32_t unused2[4];
    uint32_t CONTROL_REG;
    uint32_t IFLS;
    uint32_t IMSC;
    uint32_t unused3[2];
    uint32_t INTERRUPT_CLEAR_REG;



};

void uart_transmit(char);
char uart_receive();
void en_uart_interrupt();
void check_en_uart_interrupt();

#endif /*BS_PRAK_UART_DRIVER_H*/