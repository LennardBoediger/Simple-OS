
#ifndef BS_PRAK_KPRINTF_H
#define BS_PRAK_KPRINTF_H
#include <stdint.h>


void sent_string(char* strg);
char conv_to_ASCII(uint8_t to_hex);
void sent_hex(uint32_t num);
void sent_dez(int32_t num);
void sent_udez(uint32_t num);
void kprintf(char* text, ...);
void kprintfln(char* text, ...);

#endif //BS_PRAK_KPRINTF_H
