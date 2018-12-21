
.global k_branchto
k_branchto:
    bx      r0

.global k_branch_to_np
k_branch_to_np:
    mov     pc, #0x0

.global k_read_address
k_read_address:
    ldr     r0, [r0]
    bx      lr

.global k_write_address
k_write_address:
    str     r0, [r0]
    bx      lr
