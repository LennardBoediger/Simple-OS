#include <stdarg.h>
#include "../include/uart_driver.h"
#include "../include/init_thread.h"
#include "../syscalls/syscalls.h"


void user_sent_string(char* strg){
    while (*strg != '\0') {
        syscall_uart_print(*strg);
        strg++;
    }
}

char user_conv_to_ASCII(uint8_t to_hex) {
    char conv[] = "0123456789ABCDEF";
    if (to_hex > 15) {
        return '?';
    }
    return conv[to_hex];
}

void user_sent_hex(uint32_t num) {
    uint32_t temp_num = num;
    char hex[8];
    int8_t i;
    for (i = 0; temp_num != 0; i++) {
        uint8_t to_ASCII = temp_num % 16;
        hex[i] = user_conv_to_ASCII(to_ASCII);
        temp_num = temp_num / 16;
    }
    i--;

    user_sent_string((char*) "0x");
    int8_t nullen;
    for (nullen = 7; nullen > i; nullen--) {
        syscall_uart_print('0');
    }
    while (i >= 0) {
        syscall_uart_print(hex[i]);
        i--;
    }
}

void user_sent_dez(int32_t num) {
    uint32_t temp_num;
    char dez[10] = {0};
    int8_t i;
    if (num < 0) {
        temp_num = (uint32_t) (0 - num);
    } else temp_num = (uint32_t) num;
    for (i = 0; temp_num != 0; i++) {
        uint8_t to_ASCII = temp_num % 10;
        dez[i] = user_conv_to_ASCII(to_ASCII);
        temp_num = temp_num / 10;
    }
    if (num < 0) {
        syscall_uart_print('-');
    }
    i--;
    if (i == (-1)) {
        syscall_uart_print('0');
    }
    while (i >= 0) {
        syscall_uart_print(dez[i]);
        i--;
    }

}

void user_sent_udez(uint32_t num) {
    char udez[10] = {0};
    int8_t i;
    uint32_t temp_num = num;
    for (i = 0; temp_num != 0; i++) {
        uint8_t to_ASCII = temp_num % 10;
        udez[i] = user_conv_to_ASCII(to_ASCII);
        temp_num = temp_num / 10;
    }
    i--;
    if (i == (-1)) {
        syscall_uart_print('0');
    }
    while (i >= 0) {
        syscall_uart_print(udez[i]);
        i--;
    }
}


void uprintf(char* text, ...) {
    va_list args;
    va_start(args, text);
    char* tmp = text;
    while(*tmp != '\0') {
        if(*tmp == '%') {
            tmp++;
            switch(*tmp) {
                case 'c':
                    syscall_uart_print((unsigned char) va_arg(args, int32_t));
                    break;
                case 's':
                    user_sent_string ((char*) va_arg(args, int32_t));
                    break;
                case 'x':
                    user_sent_hex(va_arg(args, uint32_t));
                    break;
                case 'i':
                    user_sent_dez(va_arg(args, int32_t));
                    break;
                case 'u':
                    user_sent_udez(va_arg(args, uint32_t));
                    break;
                case 'p':
                    user_sent_hex(va_arg(args, uint32_t));
                    break;
                default:
                    syscall_uart_print(*tmp);
            }
            tmp++;
        }
        syscall_uart_print(*tmp);
        tmp++;
    }
    va_end(args);
}



void uprintfln(char* text, ...) {
    va_list args;
    va_start(args, text);
    char* tmp = text;
    while(*tmp != '\0') {
        if(*tmp == '%') {
            tmp++;
            switch(*tmp) {
                case 'c':
                    syscall_uart_print((unsigned char) va_arg(args, int32_t));
                    break;
                case 's':
                    user_sent_string ((char*) va_arg(args, int32_t));
                    break;
                case 'x':
                    user_sent_hex(va_arg(args, uint32_t));
                    break;
                case 'i':
                    user_sent_dez(va_arg(args, int32_t));
                    break;
                case 'u':
                    user_sent_udez(va_arg(args, uint32_t));
                    break;
                case 'p':
                    user_sent_hex(va_arg(args, uint32_t));
                    break;
                default:
                    syscall_uart_print(*tmp);
            }
            tmp++;
        }
        syscall_uart_print(*tmp);
        tmp++;
    }
    va_end(args);
    uprintf("\n\r");
}