
//Data Abort Fehler typ auslesen
.global read_dfar
read_dfar:
    mrc p15, 0, r0, c6, c0, 0   //Lese data fault address register in r0
    mov pc, lr

.global read_dfsr
read_dfsr:
    mrc p15, 0, r0, c5, c0, 0 //Lese data fault  register in r0
    mov pc, lr