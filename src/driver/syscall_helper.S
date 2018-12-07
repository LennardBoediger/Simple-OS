.global read_r8_char
read_r8_char:
    mov     r0, r8
    bx      lr

.global write_r8_char
write_r8_char:
    mov     r8, r0
    bx      lr

.global write_r5_to_r7
write_r5_to_r7:
    mov     r5, r0
    mov     r6, r1
    mov     r7, r2
    bx      lr