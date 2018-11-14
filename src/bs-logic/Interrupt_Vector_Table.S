
.global _INIT_IVT


_INIT_IVT:
    ldr r0, =_IVT
    mcr p15, 0, r0, c12, c0, 0      // Einstellen der IVT-Basisadresse
    bx lr

.global _IVT
.balign 32
_IVT:
    b reset_tr
    b undef_tr
    b swi_tr
    b prefab_tr
    b dataab_tr
    nop
    b irq_tr
    b fiq_tr


reset_tr:
    mov r0, #42
    mov r1, #42
    mov r2, #42
    mov r3, #42
    mov r4, #42
    mov r5, #42
    mov r6, #42
    mov r7, #42
    mov r8, #42
    mov r9, #42
    mov r10, #42
    mov r11, #42
    mov r12, #42
    push {r0-r15}        //Alles pushen für Registerschnappschuss
/* Printfunktion-Aufruf */
    mov r0, sp
    mrs r1, cpsr
    mrs r2, spsr
    bl reset
/* neustart des Programms*/
    b _start

undef_tr:
    mov r0, #20
    mov r1, #21
    mov r2, #22
    mov r3, #23
    mov r4, #24
    mov r5, #25
    mov r6, #26
    mov r7, #27
    mov r8, #28
    mov r9, #29
    mov r10, #30
    mov r11, #31
    mov r12, #32
    //    add lr, lr, #0      //undef befehl überspringen
    push {r0-r15}        //Alles pushen für Registerschnappschuss
/* Printfunktion-Aufruf */
    mov r0, sp
    mrs r1, cpsr
    mrs r2, spsr
    bl undef
/* Wechsel in zuvorigen Modus */
    pop {r0-r12}        //register zurück schreiben 0-14
    add sp, #4          //sp im Stack überspringen
    pop {lr}
    add sp, #4          //PC im stack überspringen
    movs pc, lr


swi_tr:
    mov r0, #20
    mov r1, #21
    mov r2, #22
    mov r3, #23
    mov r4, #24
    mov r5, #25
    mov r6, #26
    mov r7, #27
    mov r8, #28
    mov r9, #29
    mov r10, #30
    mov r11, #31
    mov r12, #32
    add lr, lr, #0      //bei #0 hängts sich auf, bei #4 springt es zu "Testbegin (...)", #8 "%b: b wird ausgegeben (...)"
    push {r0-r15}        //Alles pushen für Registerschnappschuss
/* Printfunktion-Aufruf */
    mov r0, sp
    mrs r1, cpsr
    mrs r2, spsr
    bl swi
/* Wechsel in zuvorigen Modus */
    pop {r0-r12}        //register zurück schreiben 0-14
    add sp, #4          //sp überspringen
    pop {lr}
    add sp, #4          //PC im stack überspringen
    movs pc, lr


prefab_tr:
    mov r0, #4
    mov r1, #4
    mov r2, #4
    mov r3, #4
    mov r4, #4
    mov r5, #4
    mov r6, #4
    mov r7, #4
    mov r8, #4
    mov r9, #4
    mov r10, #4
    mov r11, #4
    mov r12, #4
    sub lr, lr, #0      //TODO: wäre #4 um prefab-befehl zu wiederholen
    push {r0-r15}        //Alles pushen für Registerschnappschuss
/* Printfunktion-Aufruf */
    mov r0, sp
    mrs r1, cpsr
    mrs r2, spsr
    bl prefab
/* Wechsel in zuvorigen Modus */
    pop {r0-r12}        //register zurück schreiben 0-14
    add sp, #4          //sp überspringen
    pop {lr}
    add sp, #4          //PC im stack überspringen
    movs pc, lr

dataab_tr:
    mov r0, #5
    mov r1, #5
    mov r2, #5
    mov r3, #5
    mov r4, #5
    mov r5, #5
    mov r6, #5
    mov r7, #5
    mov r8, #5
    mov r9, #5
    mov r10, #5
    mov r11, #5
    mov r12, #5
    sub lr, lr, #4      //TODO: wäre #8 um dataab-befehl zu wiederholen
    push {r0-r15}        //Alles pushen für Registerschnappschuss
/* Printfunktion-Aufruf */
    mov r0, sp
    mrs r1, cpsr
    mrs r2, spsr
    bl dataab
/* Wechsel in zuvorigen Modus */
    pop {r0-r12}        //register zurück schreiben 0-14
    add sp, #4          //sp überspringen
    pop {lr}
    add sp, #4          //PC im stack überspringen
    movs pc, lr

/* timer und uart */
irq_tr:
    mov r0, #1
    mov r1, #0
    mov r2, #1
    mov r3, #0
    mov r4, #1
    mov r5, #0
    mov r6, #1
    mov r7, #0
    mov r8, #1
    mov r9, #0
    mov r10, #1
    mov r11, #0
    mov r12, #1
  //  sub lr, lr, #4      // TODO: mabe just use this
    sub lr, lr, #8      //zu letzten unausgeführten Befehl
    push {r0-r15}        //Alles pushen für Registerschnappschuss
/* Printfunktion-Aufruf */
    mov r0, sp
    mrs r1, cpsr
    mrs r2, spsr
    bl irq
/* Wechsel in zuvorigen Modus */
    pop {r0-r12}        //register zurück schreiben 0-14
    add sp, #4          //sp überspringen
    pop {lr}
    add sp, #4          //PC im stack überspringen
//    msr cpsr_c, #0x13           //wieder in supervisor + IRQ & FIQ mode 0
    movs pc, lr


fiq_tr:
//    stmib r13, {r0-r12}
    bl reset