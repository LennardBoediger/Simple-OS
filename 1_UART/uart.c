#include <stdarg.h>

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


/*char uart_recive(){                 //LÄUFT NOCH NICHT...
    while (!(uart_reg->FR & RXFE));  //
//    uart_transmit('r');
    char affe = (char) uart_reg->DR & 0xff;
    uart_transmit(affe);
    return affe;
}*/

void uart_transmit(char to_send){
    //while ((uart_reg->FR >> TXFF_SHIFT) & 0x01){} //wait for FIFO not full
    while (uart_reg->FR & TXFF); //wait for FIFO not full
    uart_reg->DR = to_send;
}

void sent_string(char* strg){
    while (*strg != '\0') {
        uart_transmit(*strg);
        strg++;
}

void kprintf(char* text, ...) {
    va_list args;
    va_start(args, text);
    char* tmp = text;
    while(*tmp != '\0') {
        if(*tmp == '%') {
            tmp++;
            switch(*tmp) {
                case 'c':
                    uart_transmit((unsigned char) va_arg(args, int));
                    break;
                case 's':
                    ;
                    char* strg = (char*) va_arg(args, int);
                    while (*strg != '\0') {
                        uart_transmit(*strg);
                        strg++;
                    }
                    break;
                case 'x':

                    break;
                default:
                    uart_transmit(*tmp);
            }
        tmp++;
        }
        uart_transmit(*tmp);
        tmp++;
    }
    va_end(args);
}

void echo(){
    kprintf("Test123 %s %c Test321", "peter%s", 'b');
//    while (1){
//        char tmp = uart_recive();
//        uart_transmit(tmp);
//    }

}