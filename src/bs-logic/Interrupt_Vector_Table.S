
.global _INIT_IVT


_INIT_IVT:
    ldr r0, =_IVT
    mcr p15, 0, r0, c12, c0, 0      // Einstellen der IVT-Basisadresse
    bx lr

.global _IVT
.balign 32
_IVT:
    b       reset_tr
    b       undef_tr
    b       swi_tr
    b       prefab_tr
    b       dataab_tr
    nop
    b       irq_tr
    b       fiq_tr


reset_tr:
    sub     sp, sp, #8      // 2 Felder für sp_usr und lr_usr freihalten
    push    {r13-r15}       //Alles pushen für Registerschnappschuss
    push    {r0-r12}        //wegen warning jetzt in 2 zeilen
    add     sp, sp, #72     // zurück zu den 2 freien Feldern springen
    mrs     r0, sp_usr
    mrs     r1, lr_usr
    push    {r0}            // sp_usr an unterste Stelle des IRQ_Stacks
    push    {r1}            // lr_usr an Stelle direkt darüber
    sub     sp, sp, #64     // zurück zum eigentlichen Stackende
    /* Printfunktion-Aufruf */
    mov     r0, sp
    mrs     r1, cpsr
    mrs     r2, spsr
    bl      reset
/* neustart des Programms*/
    b       _start

undef_tr:
    sub     sp, sp, #8      // 2 Felder für sp_usr und lr_usr freihalten
    push    {r13-r15}       // Alles pushen für Registerschnappschuss
    push    {r0-r12}        // wegen warning jetzt in 2 zeilen
    add     sp, sp, #72     // zurück zu den 2 freien Feldern springen
    mrs     r0, sp_usr
    mrs     r1, lr_usr
    push    {r0}            // sp_usr an unterste Stelle des IRQ_Stacks
    push    {r1}            // lr_usr an Stelle direkt darüber
    sub     sp, sp, #64     // zurück zum eigentlichen Stackende
    /* Printfunktion-Aufruf */
    mov     r0, sp
    mrs     r1, cpsr
    mrs     r2, spsr
    bl      undef
/* Wechsel in zuvorigen Modus */
    msr     spsr_cxsf, r0
    add     sp, sp, #64     // Um zuerst lr_usr und sp_usr auszulesen
    pop     {r1}
    pop     {r0}
    msr     sp_usr, r0
    msr     lr_usr, r1
    sub     sp, sp, #72
    pop     {r0-r12}        // register zurück schreiben 0-14
    add     sp, sp, #4      // sp überspringen
/* ab hier lr nicht mehr anfassen!!! Kein bl,...*/
    pop     {lr}            // lr_irq
    add     sp, sp, #4      // PC im stack überspringen
    add     sp, sp, #8      // sp wieder auf Anfang (sp_usr und lr_usr-Leichen übersprungen)
    movs    pc, lr


swi_tr:
    sub     lr, lr, #0      // EIGENTLICH #4
    sub     sp, sp, #8      // 2 Felder für sp_usr und lr_usr freihalten
    push    {r13-r15}       // Alles pushen für Registerschnappschuss
    push    {r0-r12}        // wegen warning jetzt in 2 zeilen
    add     sp, sp, #72     // zurück zu den 2 freien Feldern springen
    mrs     r0, sp_usr
    push    {r0}            // sp_usr an unterste Stelle des IRQ_Stacks
    mrs     r1, lr_usr
    push    {r1}            // lr_usr an Stelle direkt darüber
    sub     sp, sp, #64     // zurück zum eigentlichen Stackende
    /* Printfunktion-Aufruf */
    mov     r0, sp
    mrs     r1, cpsr
    mrs     r2, spsr
    bl      swi_interrupt
/* Wechsel in zuvorigen Modus */
    msr     spsr_cxsf, r0
    add     sp, sp, #64     // Um zuerst lr_usr und sp_usr auszulesen
    pop     {r1}
    pop     {r0}
    msr     sp_usr, r0
    msr     lr_usr, r1
    sub     sp, sp, #72
    pop     {r0-r12}        // register zurück schreiben 0-14
    add     sp, sp, #4      // sp überspringen
/* ab hier lr nicht mehr anfassen!!! Kein bl,...*/
    pop     {lr}            // lr_irq
    add     sp, sp, #4      // PC im stack überspringen
    add     sp, sp, #8      // sp wieder auf Anfang (sp_usr und lr_usr-Leichen übersprungen)
    movs    pc, lr


prefab_tr:
    sub     lr, lr, #0      // wäre #4 um prefab-befehl zu wiederholen
    sub     sp, sp, #8      // 2 Felder für sp_usr und lr_usr freihalten
    push    {r13-r15}       // Alles pushen für Registerschnappschuss
    push    {r0-r12}        // wegen warning jetzt in 2 zeilen
    add     sp, sp, #72     // zurück zu den 2 freien Feldern springen
    mrs     r0, sp_usr
    mrs     r1, lr_usr
    push    {r0}            // sp_usr an unterste Stelle des IRQ_Stacks
    push    {r1}            // lr_usr an Stelle direkt darüber
    sub     sp, sp, #64     // zurück zum eigentlichen Stackende
    /* Printfunktion-Aufruf */
    mov     r0, sp
    mrs     r1, cpsr
    mrs     r2, spsr
    bl      prefab
/* Wechsel in zuvorigen Modus */
    msr     spsr_cxsf, r0
    add     sp, sp, #64     // Um zuerst lr_usr und sp_usr auszulesen
    pop     {r1}
    pop     {r0}
    msr     sp_usr, r0
    msr     lr_usr, r1
    sub     sp, sp, #72
    pop     {r0-r12}        // register zurück schreiben 0-14
    add     sp, sp, #4      // sp überspringen
/* ab hier lr nicht mehr anfassen!!! Kein bl,...*/
    pop     {lr}            // lr_irq
    add     sp, sp, #4      // PC im stack überspringen
    add     sp, sp, #8      // sp wieder auf Anfang (sp_usr und lr_usr-Leichen übersprungen)
    movs    pc, lr

dataab_tr:
    sub     lr, lr, #4      // wäre #8 um dataab-befehl zu wiederholen
    sub     sp, sp, #8      // 2 Felder für sp_usr und lr_usr freihalten
    push    {r13-r15}       // Alles pushen für Registerschnappschuss
    push    {r0-r12}        // wegen warning jetzt in 2 zeilen
    add     sp, sp, #72     // zurück zu den 2 freien Feldern springen
    mrs     r0, sp_usr
    mrs     r1, lr_usr
    push    {r0}            // sp_usr an unterste Stelle des IRQ_Stacks
    push    {r1}            // lr_usr an Stelle direkt darüber
    sub     sp, sp, #64     // zurück zum eigentlichen Stackende
    /* Printfunktion-Aufruf */
    mov     r0, sp
    mrs     r1, cpsr
    mrs     r2, spsr
    bl      dataab
/* Wechsel in zuvorigen Modus */
    msr     spsr_cxsf, r0
    add     sp, sp, #64     // Um zuerst lr_usr und sp_usr auszulesen
    pop     {r1}
    pop     {r0}
    msr     sp_usr, r0
    msr     lr_usr, r1
    sub     sp, sp, #72
    pop     {r0-r12}        // register zurück schreiben 0-14
    add     sp, sp, #4      // sp überspringen
/* ab hier lr nicht mehr anfassen!!! Kein bl,...*/
    pop     {lr}            // lr_irq
    add     sp, sp, #4      // PC im stack überspringen
    add     sp, sp, #8      // sp wieder auf Anfang (sp_usr und lr_usr-Leichen übersprungen)
    movs    pc, lr

/* timer und uart */
irq_tr:
    sub     lr, lr, #4      // zu letzten unausgeführten Befehl
    sub     sp, sp, #8      // 2 Felder für sp_usr und lr_usr freihalten
    push    {r13-r15}       //Alles pushen für Registerschnappschuss
    push    {r0-r12}
    add     sp, sp, #72     // zurück zu den 2 freien Feldern springen
    mrs     r0, sp_usr
    mrs     r1, lr_usr
    push    {r0}            // sp_usr an unterste Stelle des IRQ_Stacks
    push    {r1}            // lr_usr an Stelle direkt darüber
    sub     sp, sp, #64     // zurück zum eigentlichen Stackende
    /* Printfunktion-Aufruf */
    mov     r0, sp
    mrs     r1, cpsr
    mrs     r2, spsr
    bl      irq
    cmp     r0, #0
    beq     j_if_uart
    msr     spsr_cxsf, r0
j_if_uart:
    /* Wechsel in zuvorigen Modus */
    add     sp, sp, #64     // Um zuerst lr_usr und sp_usr auszulesen
    pop     {r1}
    pop     {r0}
    msr     sp_usr, r0
    msr     lr_usr, r1
    sub     sp, sp, #72
    pop     {r0-r12}        // register zurück schreiben 0-14
    add     sp, sp, #4      // sp überspringen
/* ab hier lr nicht mehr anfassen!!! Kein bl,...*/
    pop     {lr}            // lr_irq
    add     sp, sp, #4      // PC im stack überspringen
    add     sp, sp, #8      // sp wieder auf Anfang (sp_usr und lr_usr-Leichen übersprungen)
    movs    pc, lr


fiq_tr:
//    stmib r13, {r0-r12}
    bl      reset