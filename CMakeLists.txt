cmake_minimum_required(VERSION 3.9)
project(Bs_Prak)

set(CMAKE_CXX_STANDARD 11)

add_executable(Bs_Prak
        1_UART/kernel
        1_UART/kernel.bin
        1_UART/kernel.lds
        1_UART/kernel_dump.s
        1_UART/Makefile
        1_UART/namen.txt
        1_UART/start.d
        1_UART/start.S
        1_UART/uart.c
        README.md)
