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
}

char conv_to_hex(int to_hex) {
    switch (to_hex) {
        case 0:
            return '0';
        case 1:
            return '1';
        case 2:
            return '2';
        case 3:
            return '3';
        case 4:
            return '4';
        case 5:
            return '5';
        case 6:
            return '6';
        case 7:
            return '7';
        case 8:
            return '8';
        case 9:
            return '9';
        case 10:
            return 'A';
        case 11:
            return 'B';
        case 12:
            return 'C';
        case 13:
            return 'D';
        case 14:
            return 'E';
        case 15:
            return 'F';
    }
}

void sent_hex(unsigned int num){
    int to_hex = num % 16;
    char hex = conv_to_hex(to_hex);

    uart_transmit()
}        case 14:
            return 'E';


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
                    sent_string ((char*) va_arg(args, int));
                    break;
                case 'x':
                    sent_hex(va_arg(args, unsigned int));
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