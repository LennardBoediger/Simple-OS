.global force_dataab

force_dataab:
    mrc     p15, 0, r0, c1, c0, 0
    orr     r0, #2
    mcr     p15, 0, r0, c1, c0, 0
    mov     r0, #0x1
    ldr     r0, [r0]
    mov pc, lr