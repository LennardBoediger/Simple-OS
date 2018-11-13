//
// Created by pablo on 13.11.18.
//

#ifndef BS_PRAK_MODE_REG_HELPER_H
#define BS_PRAK_MODE_REG_HELPER_H
#include <stdint.h>

uint32_t get_lr_system();
uint32_t get_sp_system();
uint32_t get_lr_supervisor();
uint32_t get_sp_supervisor();
uint32_t get_spsr_supervisor();
uint32_t get_lr_abort();
uint32_t get_sp_abort();
uint32_t get_spsr_abort();
uint32_t get_lr_FIQ();
uint32_t get_sp_FIQ();
uint32_t get_spsr_FIQ();
uint32_t get_lr_IRQ();
uint32_t get_sp_IRQ();
uint32_t get_spsr_IRQ();
uint32_t get_lr_undefined();
uint32_t get_sp_undefined();
uint32_t get_spsr_undefined();


#endif //BS_PRAK_MODE_REG_HELPER_H
