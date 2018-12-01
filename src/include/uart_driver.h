
#ifndef BS_PRAK_UART_DRIVER_H
#define BS_PRAK_UART_DRIVER_H
#include <stdint.h>

struct uart {
    uint32_t DR;
    uint32_t rsrecr;
    uint32_t unused[4];
    uint32_t FR;
    uint32_t notdefined1;
    uint32_t ilpr;
    uint32_t ibrd;
    uint32_t fbrd;
    uint32_t lcrh;
    uint32_t CONTROL_REG;
    uint32_t IFLS;
    uint32_t IMSC;
    uint32_t RIS;
    uint32_t MIS;
    uint32_t ICR;
};

void uart_transmit(char);
void uart_receive();
char read_uart_buffer();
void en_uart_interrupt();

#endif /*BS_PRAK_UART_DRIVER_H*/