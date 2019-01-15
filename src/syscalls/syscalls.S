.global syscall_kill_thread
syscall_kill_thread:
    swi     0
    bx      lr

.global syscall_prepare_thread
syscall_prepare_thread:
    swi     1
    bx      lr

.global syscall_sleep_thread
syscall_sleep_thread:
    swi     2
    bx      lr

.global syscall_uart_print
syscall_uart_print:
    swi     3
    bx      lr

.global syscall_uart_read
syscall_uart_read:
    swi     4
    bx      lr

.global syscall_prepare_process
syscall_prepare_process:
    swi     5
    bx      lr