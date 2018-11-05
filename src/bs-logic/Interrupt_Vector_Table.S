.global _IVT
.global _INIT_IVT
.global reset_tr

_INIT_IVT:
    ldr r0, _IVT
    mcr p15, 0, r0, c12, c0, 0
    bx lr

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
    bl reset            //testweise
//    push {r0-r12}
//    bl reset
    //    mov r13, #(1024*1024*128)

undef_tr:
    bl undef            //testweise
/*    push {r0-r12}
    bl undef
*/

swi_tr:
//    stmib r13, {r0-r12}
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