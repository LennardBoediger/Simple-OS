#include "../include/uart_driver.h"
#include "../include/kprintf.h"
#define UART_BASE (0x7E201000 - 0x3F000000)
#define RXFE_SHIFT 4
#define TXFF_SHIFT 5
#define RTIM_SHIFT 6
#define MAX_BUFFER_SIZE 64
#define RXFE (1 << RXFE_SHIFT)
#define TXFF (1 << TXFF_SHIFT)

char uart_buffer[MAX_BUFFER_SIZE];
uint32_t current_uart_buffer_position;

static volatile
struct uart * const uart_reg = (struct uart *)UART_BASE;


void write_uart_buffer (char input) {
    uart_buffer[current_uart_buffer_position] = input;
    if (current_uart_buffer_position < MAX_BUFFER_SIZE-1) {
        current_uart_buffer_position++;
    } else {
        current_uart_buffer_position = 0;
    }
}

char read_uart_buffer() {
    char temp = uart_buffer[current_uart_buffer_position];
    if (current_uart_buffer_position > 0) {
        current_uart_buffer_position--;
    } else {
        current_uart_buffer_position = MAX_BUFFER_SIZE-1;
    }
        return temp;
}

void uart_receive() {
    if ((uart_reg->FR & RXFE) == 0x00) {
        char in = (char) uart_reg->DR & 0xff;
        write_uart_buffer(in);
    }
}

void uart_transmit(char to_send){
    while (uart_reg->FR & TXFF);
    uart_reg->DR = to_send;
}

void en_uart_interrupt() {
    current_uart_buffer_position = 0;
    uart_reg->IMSC |= (1 << RTIM_SHIFT);
}