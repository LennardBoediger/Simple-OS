cmake_minimum_required(VERSION 3.9)
project(Bs_Prak)

set(CMAKE_CXX_STANDARD 11)

add_executable(Bs_Prak
        src/driver/uart_driver.c
        src/include/uart_driver.h
        src/lib/printf_lib.c
        src/bs-logic/Interrupt_Handler.c
        src/include/Interrupt_Handler.h src/bs-logic/inputtest.c src/include/printf_lib.h src/driver/interrupt_regs_driver.c src/include/interrupt_regs_driver.h src/include/dataab_helper.h src/include/mode_reg_helper.h src/lib/Interrupt_printer.c src/bs-logic/threads/init_thread.c src/bs-logic/threads/threads_handler.c src/include/threads_handler.h src/include/init_thread.h src/lib/userthread.c src/include/userthread.h src/user/uart_listen.c src/user/include/uart_listen.h src/driver/syscalls.c src/include/syscalls.h src/include/syscall_helper.h src/user/uprintf.c src/bs-logic/kprintf.c src/include/kprintf.h src/user/include/uprintf.h)
