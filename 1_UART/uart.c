#include <stdarg.h>
#include "uart_driver.h"



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
        default:
            return '!';
    }
}

void sent_hex(unsigned int num) {
    unsigned int temp_num = num;
    char hex[8];
    int i;
    for (i = 0; temp_num != 0; i++) {
        int to_hex = temp_num % 16;
        hex[i] = conv_to_hex(to_hex);
        temp_num = temp_num / 16;
    }
    i--;
    sent_string((char*) "0x");
    while (i >= 0) {
        uart_transmit(hex[i]);
        i--;
    }
}

void sent_dez(int num) {
    int temp_num = num;
    char dez[11];           //11. Zeichen für Minuszeichen
    int i;
    if (temp_num < 0) {
        dez[0] = '-';
        temp_num = 0 - temp_num;
    }
    for (i = 1; temp_num != 0; i++) {
        int to_hex = temp_num % 10;     //kann verwendet werden, da Rest nie >9
        dez[i] = conv_to_hex(to_hex);
        temp_num = temp_num / 10;
    }
    i--;
    while (i >= 0) {
        uart_transmit(dez[i]);
        i--;
    }
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
                    sent_string ((char*) va_arg(args, int));
                    break;
                case 'x':
                    sent_hex(va_arg(args, unsigned int));
                    break;
                case 'i':
                    sent_dez(va_arg(args, int));
                    break;
                case 'u':
                //    sent_
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
    kprintf("Testbegin\n\r");
    kprintf("%%c: b wird ausgegeben: %c\n\r", 'b');
    kprintf("%%s: Die Welt %s", "ist schön!\n\r");
    kprintf("%%x: 47096 hexadezimal gleich %x\n\r", 47096);
    kprintf("%%i: kleinste negative Zahl: %i \n\r", -2147483648);
    kprintf("%%i: groesste positive Zahl: %i\n\r", 2147483647);
    kprintf("%%u: groesste unsigned Int: %u\n\r", 4294967295);


//    while (1){
//        char tmp = uart_recive();
//        uart_transmit(tmp);
//    }
}