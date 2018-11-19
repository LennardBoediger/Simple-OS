
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
   push {r13-r15}        //Alles pushen für Registerschnappschuss
   push {r0-r12}        //wegen warning jetzt in 2 zeilen
/* Printfunktion-Aufruf */
    mov r0, sp
    mrs r1, cpsr
    mrs r2, spsr
    bl reset
/* neustart des Programms*/
    b _start

undef_tr:
    //    add lr, lr, #0      //undef befehl überspringen
    push {r13-r15}        //Alles pushen für Registerschnappschuss
    push {r0-r12}        //wegen warning jetzt in 2 zeilen
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
    add lr, lr, #0
    push {r13-r15}        //Alles pushen für Registerschnappschuss
    push {r0-r12}        //wegen warning jetzt in 2 zeilen
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
    sub lr, lr, #0       //wäre #4 um prefab-befehl zu wiederholen
    push {r13-r15}       //Alles pushen für Registerschnappschuss
    push {r0-r12}        //wegen warning jetzt in 2 zeilen
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
    sub lr, lr, #4      //wäre #8 um dataab-befehl zu wiederholen
    push {r13-r15}      //Alles pushen für Registerschnappschuss
    push {r0-r12}       //wegen warning jetzt in 2 zeilen
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
    sub lr, lr, #4      // zu letzten unausgeführten Befehl
    sub sp, #8          // 2 Felder für sp_usr und lr_usr freihalten
    //    push {sp_usr}       //NEU AUFGABE 3 FÜR THREAD-WECHSEL
    //    push {lr_usr}       //NEU AUFGABE 3 FÜR THREAD-WECHSEL
    push {r13-r15}      //Alles pushen für Registerschnappschuss
    push {r0-r12}       //wegen warning jetzt in 2 zeilen
    add sp, #72         // zurück zu den 2 freien Feldern springen
    mrs r0, sp_usr
    mrs r1, lr_usr
    push {r0}           // sp_usr an unterste Stelle des IRQ_Stacks
    push {r1}           // lr_usr an Stelle direkt darüber
    sub sp, #64         // zurück zum eigentlichen Stackende
    /* Printfunktion-Aufruf */
    mov r0, sp
    mrs r1, cpsr
    mrs r2, spsr
    //    mov r3, pc+8
    bl irq
    cmp r0, #0          //TODO: testen
    beq j_if_uart       //TODO: testen
    //    IT EQ r0, #0
    //    T j_if_uart E
    //    cbz r0, j_if_uart
    msr spsr, r0
    /* Wechsel in zuvorigen Modus */
j_if_uart:
    add sp, #64
    pop {r1}
    pop {r0}
    msr sp_usr, r0
    msr lr_usr, r1
    sub sp, #72
    pop {r0-r12}        //register zurück schreiben 0-14
    add sp, #4          //sp überspringen
    pop {lr}
    add sp, #4          //PC im stack überspringen
    add sp, #8          // sp wieder auf Anfang
//    pop {lr_usr}        //NEU AUFGABE 3 FÜR THREAD-WECHSEL
//    pop {sp_usr}        //NEU ...
    movs pc, lr


fiq_tr:
//    stmib r13, {r0-r12}
    bl reset