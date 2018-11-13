#include "../include/uart_driver.h"
#include "../include/kprintf.h"
#define UART_BASE (0x7E201000 - 0x3F000000)
#define TXFF_SHIFT 5
#define RXFE_SHIFT 4
#define TXFF (1 << TXFF_SHIFT)
#define RXFE (1 << RXFE_SHIFT)

#define RTIM_SHIFT 6
#define RXE 9
#define TXE 8



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

void en_uart_interrupt() {
      uart_reg->CONTROL_REG |= (1 << TXE);
      uart_reg->CONTROL_REG |= (1 << RXE);
 //   uart_reg->CONTROL_REG |= (1 << 0);
    uart_reg->IMSC |= (1 << RTIM_SHIFT);
 //   check_en_uart_interrupt();
}

void check_en_uart_interrupt() {
    if (uart_reg->CONTROL_REG & (1 << RXE) >> RXE) {
        kprintf("receive ist enabled\n\r");
        if (uart_reg->CONTROL_REG & (1 << TXE) >> TXE) {
            kprintf("send ist enabled\n\r");
        }
    } else {
        kprintf("###### ERROR UART NICHT ENABLED #######\n\r");
        en_uart_interrupt();
    }
}
