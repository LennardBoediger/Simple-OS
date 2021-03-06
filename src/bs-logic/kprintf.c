#include <stdarg.h>
#include "../include/printf_lib.h"
#include "../include/uart_driver.h"


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