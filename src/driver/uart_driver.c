#include "../include/uart_driver.h"
#include "../include/printf_lib.h"
#define UART_BASE (0x7E201000 - 0x3F000000)
#define RXFE_SHIFT 4
#define TXFF_SHIFT 5
#define RTIM_SHIFT 6
#define MAX_BUFFER_SIZE 64
#define RXFE (1 << RXFE_SHIFT)
#define TXFF (1 << TXFF_SHIFT)

char uart_buffer[MAX_BUFFER_SIZE];
int32_t current_uart_buffer_position;

static volatile
struct uart * const uart_reg = (struct uart *)UART_BASE;


void write_uart_buffer (char input) {
    if (current_uart_buffer_position < MAX_BUFFER_SIZE-1) {
        uart_buffer[current_uart_buffer_position] = input;
        current_uart_buffer_position++;
    } else {
        kprintfln("WRITE_UART_BUFFER -> Buffer voll");
    }
}

char read_uart_buffer() {
    if (current_uart_buffer_position > 0) {
        current_uart_buffer_position--;
        char temp = uart_buffer[current_uart_buffer_position];
        return temp;
    } else {
        return 0;
    }
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
    uint16_t i;
    for (i = 0; i < MAX_BUFFER_SIZE; i++) {
        uart_buffer[i] = 0;
    }
    current_uart_buffer_position = 0;
    uart_reg->IMSC |= (1 << RTIM_SHIFT);
}