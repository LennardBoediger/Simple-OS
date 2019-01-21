#include <stdint.h>
#include "../include/kprintf.h"

void mymalloc(void* dst, uint32_t irq_stack_data_size) {
    uint8_t *dst_byte = (uint8_t *) dst;
    uint32_t j;
    for (j = 0; j < irq_stack_data_size; j++) {
        *dst_byte = 0;
        dst_byte++;
    }
}

void memcopy(void* src, void* dst, uint32_t irq_stack_data_size) {
    uint8_t *src_byte = (uint8_t *) src;
    uint8_t *dst_byte = (uint8_t *) dst;
    uint32_t j;
    for (j = 0; j < irq_stack_data_size; j++) {
        *dst_byte = *src_byte;
        src_byte++;
        dst_byte++;
    }
}
