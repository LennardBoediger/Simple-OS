#include <stdarg.h>
#include "../include/uart_driver.h"
#include "../include/kprintf.h"


void sent_string(char* strg){
    while (*strg != '\0') {
        uart_transmit(*strg);
        strg++;
    }
}

char conv_to_ASCII(uint8_t to_hex) {
    char conv[] = "0123456789ABCDEF";
    if (to_hex > 15) {
        return '?';
    }
    return conv[to_hex];
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
    int8_t nullen;
    for (nullen = 7; nullen > i; nullen--) {
        uart_transmit('0');
    }
    while (i >= 0) {
        uart_transmit(hex[i]);
        i--;
    }
}

void sent_dez(int32_t num) {
    uint32_t temp_num;
    char dez[10] = {0};
    int8_t i;
    if (num < 0) {
        temp_num = (uint32_t) (0 - num);
    } else temp_num = (uint32_t) num;
    for (i = 0; temp_num != 0; i++) {
        uint8_t to_ASCII = temp_num % 10;
        dez[i] = conv_to_ASCII(to_ASCII);
        temp_num = temp_num / 10;
    }
    if (num < 0) {
        uart_transmit('-');
    }
    i--;
    if (i == (-1)) {
        uart_transmit('0');
    }
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
    if (i == (-1)) {
        uart_transmit('0');
    }
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
                case 'p':
                    sent_hex(va_arg(args, uint32_t));
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

void kprintfln(char* text, ...) {
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
                case 'p':
                    sent_hex(va_arg(args, uint32_t));
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
    kprintf("\n\r");
}