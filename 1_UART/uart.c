#define UART_BASE (0x7E201000 - 0x3F000000)
#define TXFF_SHIFT 5
#define RXFE_SHIFT 4
#define TXFF (1 << TXFF_SHIFT)
#define RXFE (1 << RXFE_SHIFT)

struct uart {
    unsigned int DR;
    unsigned int UNUSED[5]; //lennard hat das nicht verstanden
    unsigned int FR;

};

static volatile
struct uart * const uart_reg = (struct uart *)UART_BASE;




/*int init() {
     Initialisieren  //to do set flags//
    gpio_port->func[0] = gpio_output << (YELLOW_LED * GPF_BITS);
}*/


char uart_recive(){
    while (!(uart_reg->FR & RXFE));
    uart_transmit('r');
    char affe = (char) uart_reg->DR & 0xff;
    uart_transmit(affe);
    return affe;
}

void uart_transmit(char to_send){
    //while ((uart_reg->FR >> TXFF_SHIFT) & 0x01){} //wait for FIFO not full
    while (uart_reg->FR & TXFF); //wait for FIFO not full
    uart_reg->DR = to_send;
}

void echo(){
    uart_transmit('c');
    while (1){
        char tmp = uart_recive();
        uart_transmit(tmp);
    }

}
