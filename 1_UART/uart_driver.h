#ifndef BS_PRAK_UART_DRIVER_H
#define BS_PRAK_UART_DRIVER_H

struct uart {
    unsigned int DR;
    unsigned int UNUSED[5]; //lennard hat das nicht verstanden
    unsigned int FR;

};

void uart_transmit(char);

#endif //BS_PRAK_UART_DRIVER_H
