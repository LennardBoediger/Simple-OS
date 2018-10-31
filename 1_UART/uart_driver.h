#ifndef BS_PRAK_UART_DRIVER_H
#define BS_PRAK_UART_DRIVER_H
#include <stdint.h>

struct uart {
    uint32_t DR;
    uint32_t UNUSED[5];
    uint32_t FR;

};

void uart_transmit(char);
char uart_receive();

#endif BS_PRAK_UART_DRIVER_H
