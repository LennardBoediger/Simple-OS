#include "../include/uart_driver.h"
#include "../include/kprintf.h"
#define UART_BASE (0x7E201000 - 0x3F000000)
#define RXFE_SHIFT 4
#define TXFF_SHIFT 5
#define RTIM_SHIFT 6
#define RXFE (1 << RXFE_SHIFT)
#define TXFF (1 << TXFF_SHIFT)


static volatile
struct uart * const uart_reg = (struct uart *)UART_BASE;

char uart_receive() {
    if ((uart_reg->FR & RXFE) == 0x00) {
        char in = (char) uart_reg->DR & 0xff;
        return in;
    }
    return 0;
}

void uart_transmit(char to_send){
    while (uart_reg->FR & TXFF);
    uart_reg->DR = to_send;
}

void en_uart_interrupt() {
    uart_reg->IMSC |= (1 << RTIM_SHIFT);
}