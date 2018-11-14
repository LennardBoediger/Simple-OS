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
/*
void disable_uart(){
    while(//uart_busy bit 3);
   //disable uart: write 0 to: uart_reg->CONTROL_REG &&&&& (1 << 0)
    while(//uart_busy bit 3);
    //flush FIFO
    // write 0 to uart_reg->lcrh reg at 4
    // write 1 to uart_reg->lcrh reg at 4
}

void enable_uart(){
    uart_reg->CONTROL_REG |= (1 << 0);
 }
*/

//TODO: zeile 52 bring nix weil wir uartt nicht disabeln!?!?!
void en_uart_interrupt() {
    kprintf("TODO: Initializing UART:\r\n");
    // disable_uart();
  //  uart_reg->CONTROL_REG |= (1 << TXE);
 //   uart_reg->CONTROL_REG |= (1 << RXE);
    //disable_fifo(); wenn wir ein rinbuffer machen
    uart_reg->IMSC |= (1 << RTIM_SHIFT); // enable interrupt an bit 6
 //   uart_reg->CONTROL_REG |= (1 << 0); // enable uart
    //   check_en_uart_interrupt();

}

void check_en_uart_interrupt() {
    kprintf("dont print");

  /*  if (uart_reg->CONTROL_REG & (1 << RXE) >> RXE) {
        kprintf("receive ist enabled\n\r");
        if (uart_reg->CONTROL_REG & (1 << TXE) >> TXE) {
            kprintf("send ist enabled\n\r");
        }
    } else {
        kprintf("###### ERROR UART NICHT ENABLED #######\n\r");
        en_uart_interrupt();
    }*/
}
