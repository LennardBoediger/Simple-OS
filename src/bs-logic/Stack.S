.global INIT_STACK

//wechselt durch die Modi und schreibt deren Stackpointer
INIT_STACK:
    msr cpsr_c, #0xDF           //sys 11011111
    ldr sp, =#(128*1024*1019)
    msr cpsr_c, #0xDB           //undef 11011011
    ldr sp, =#(128*1024*1020)
    msr cpsr_c, #0xD7           //abort 11010111
    ldr sp, =#(128*1024*1021)
    msr cpsr_c, #0xD3           //supervisor 11010011
    ldr sp, =#(128*1024*1022)
    msr cpsr_c, #0xD2           //IRQ 11010010
    ldr sp, =#(128*1024*1023)
    msr cpsr_c, #0xD1           //FIQ 11010001
    ldr sp, =#(128*1024*1024)
    msr cpsr_c, #0x13           //wieder in supervisor + IRQ & FIQ mode 0
    bx lr
