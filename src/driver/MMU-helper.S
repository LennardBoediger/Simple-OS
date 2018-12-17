

.global init_dacr
init_dacr:
    mcr     p15, 0, r0, c3, c0, 0
    bx      lr

.global init_ttbcr
init_ttbcr:
    mcr     p15, 0, r0, c2, c0, 2
    bx      lr

.global init_ttbr0
init_ttbr0:
    mcr     p15, 0, r0, c2, c0, 0
    bx      lr

.global get_sctlr
get_sctlr:
    mrc     p15, 0, r0, c1, c0, 0
    bx lr

.global set_sctlr
set_sctlr:
    mcr     p15, 0, r0, c1, c0, 0
    bx lr