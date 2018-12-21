#define undef .word 0xe7ffffff

.global stack_overflow
stack_overflow:
    push    {r0}
    b       stack_overflow
    bx      lr

.global branch_to_np
branch_to_np:
    mov     pc, #0x0

.global read_address
read_address:
    ldr     r0, [r0]
    bx      lr

.global write_address
write_address:
    str     r0, [r0]
    bx      lr



/********************/
.global branchto
branchto:
        bx r0

        //stack overflow.com sagt LDR R1, [R0]; MOV PC, R1 //TODO

