cmake_minimum_required(VERSION 3.9)
project(Bs_Prak)

set(CMAKE_CXX_STANDARD 11)

add_executable(Bs_Prak
        src/driver/uart_driver.c
        src/include/uart_driver.h
        src/lib/kprintf.c
        src/bs-logic/Interrupt_Handler.c
        src/include/Interrupt_Handler.h src/bs-logic/inputtest.c src/include/kprintf.h src/driver/interrupt_regs_driver.c src/include/interrupt_regs_driver.h src/include/f_dataab.h)
