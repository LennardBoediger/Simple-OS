.global INIT_STACK

INIT_STACK:
    msr cpsr_c, #0xDF           //sys 11011111
    mov sp, #(123*1024*1024)
    msr cpsr_c, #0xDB           //undef 11011011
    mov sp, #(124*1024*1024)
    msr cpsr_c, #0xD7           //abort 11010111
    mov sp, #(125*1024*1024)
    msr cpsr_c, #0xD3           //supervisor 11010011
    mov sp, #(126*1024*1024)
    msr cpsr_c, #0xD2           //IRQ 11010010b
    mov sp, #(127*1024*1024)
    msr cpsr_c, #0xD1           //FIQ 11010001
    mov sp, #(128*1024*1024)
    msr cpsr_c, #0xD3           //wieder in supervisor, wie beim Eintritt
    bx lr