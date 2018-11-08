
.global _INIT_IVT
//.global reset_tr
//.global undef_tr

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
//    b reset

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
    push {r0-r15}        //Alles pushen für Registerschnappschuss
    mov r0, sp
    bl undef
    add sp, sp, #4       //r15 überspringen
    pop {lr}             //r14 stimmt so, da bei undef Folgebefehl ausgeführt wird
    add sp, sp, #4       //r13 überspringen
    pop {r0-r12}
    msr cpsr_c, #0x13           //wieder in supervisor + IRQ & FIQ mode 0



swi_tr:
    stmib r13, {r0-r12}
    bl reset

prefab_tr:
//    stmib r13, {r0-r12}
    bl reset

dataab_tr:
//    stmib r13, {r0-r12}
    bl reset

irq_tr:
//    stmib r13, {r0-r12}
    bl reset

fiq_tr:
//    stmib r13, {r0-r12}
    bl reset