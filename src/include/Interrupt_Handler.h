//
// Created by Lennard on 02.11.18.
//
#ifndef BS_PRAK_INTERRUPT_HANDLER_H
#define BS_PRAK_INTERRUPT_HANDLER_H

//128mb RAM size
//1kb stack size
#define USER_STACK  128*1024*1024
#define FIQ         128*1023*1024
#define IRQ         128*1022*1024
#define SUPERVISOR  128*1021*1024
#define ABORT       128*1020*1024
#define UNDEFINED   128*1019*1024
#define SYS         128*1018*1024

#endif //BS_PRAK_INTERRUPT_HANDLER_H
