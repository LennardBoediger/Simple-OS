.global read_r8_char
read_r8_char:
    mov     r0, r8
    bx      lr

.global write_r8_char
write_r8_char:
    mov     r8, r0
    bx      lr