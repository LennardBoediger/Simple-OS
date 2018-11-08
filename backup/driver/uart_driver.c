#include "../include/uart_driver.h"
#define UART_BASE (0x7E201000 - 0x3F000000)
#define TXFF_SHIFT 5
#define RXFE_SHIFT 4
#define TXFF (1 << TXFF_SHIFT)
#define RXFE (1 << RXFE_SHIFT)

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
    //while ((uart_reg->FR >> TXFF_SHIFT) & 0x01){} //wait for FIFO not full
    while (uart_reg->FR & TXFF); //wait for FIFO not full
    uart_reg->DR = to_send;
}