#include <stdarg.h>
#include "uart_driver.h"



void sent_string(char* strg){
    while (*strg != '\0') {
        uart_transmit(*strg);
        strg++;
    }
}

char conv_to_ASCII(uint8_t to_hex) {
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

void sent_hex(uint32_t num) {
    uint32_t temp_num = num;
    char hex[8];
    int8_t i;
    for (i = 0; temp_num != 0; i++) {
        uint8_t to_ASCII = temp_num % 16;
        hex[i] = conv_to_ASCII(to_ASCII);
        temp_num = temp_num / 16;
    }
    i--;
    sent_string((char*) "0x");
    while (i >= 0) {
        uart_transmit(hex[i]);
        i--;
    }
}

void sent_dez(int32_t num) {
    uint32_t temp_num;
    char dez[10] = {0};           //11. Zeichen für Minuszeichen
    int8_t i;
    if (num < 0) {
        temp_num = (uint32_t) (0 - num);
    } else temp_num = (uint32_t) num;
    for (i = 0; temp_num != 0; i++) {
        uint8_t to_ASCII = temp_num % 10;     //kann verwendet werden, da Rest nie >9
        dez[i] = conv_to_ASCII(to_ASCII);
        temp_num = temp_num / 10;
    }
    if (num < 0) {
        uart_transmit('-');
    }
    i--;
    while (i >= 0) {
        uart_transmit(dez[i]);
        i--;
    }

}

void sent_udez(uint32_t num) {
    char udez[10] = {0};
    int8_t i;
    uint32_t temp_num = num;
    for (i = 0; temp_num != 0; i++) {
        uint8_t to_ASCII =  temp_num % 10;
        udez[i] = conv_to_ASCII(to_ASCII);
        temp_num = temp_num / 10;
    }
    i--;
    while (i >= 0) {
        uart_transmit(udez[i]);
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
                    uart_transmit((unsigned char) va_arg(args, int32_t));
                    break;
                case 's':
                    sent_string ((char*) va_arg(args, int32_t));
                    break;
                case 'x':
                    sent_hex(va_arg(args, uint32_t));
                    break;
                case 'i':
                    sent_dez(va_arg(args, int32_t));
                    break;
                case 'u':
                    sent_udez(va_arg(args, uint32_t));
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