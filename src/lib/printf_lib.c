#include "../include/uart_driver.h"
#include "../include/printf_lib.h"


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