.section .init

.global _start
_start:

	/* Erkennen ob Hypervisor Mode aktiv */
	mrs r0, cpsr
	and r0, r0, #0x1F
	mov r1, #0x1A	//Modebits = 0x1A = Hypervisor Mode
	cmp r0, r1		//Vergleichen und Statusbits setzen für bedingte Sprünge
	
	/* Wenn Hypemode erkannt: abschalten (sprung nach _exitHyper)
	 * wenn nicht erkannt: dann weitermachen (weiterlaufen nach _bsprak)
	 */
	beq _exitHyper
	 
_bsprak:
	
	/* Einsprung ins Hauptprogramm */
    bl      _INIT_IVT
    bl      INIT_STACK
    bl      enable_ext_interrupts
    bl      initialize_timer
/* lößt prefetch abort aus */
//    bkpt
    loop:
    b loop
	bl      inputtest

.Lend:
	b       .Lend


_exitHyper:

	/* R�cksprungadresse ins Hypervisor R�cksprungregister schreiben.
	 * Dies ist das Label _bsprak f�r die normale Programmausf�rung.
	 */
	ldr lr, =_bsprak
	msr ELR_hyp, lr

	/* Der Hypervisor mode ist auch "nur" eine etwas kompliziertere Exception.
	 * Also springen wir aus dieser zur�ck.
	 * (R�cksprunge in Aufgabe 2 sind anders zu l�sen!).
	 * Dazu zun�chst das CPSR auslesen und die Modebits zum Supervisor �ndern.
	 * Danach dies in das SPSR f�r den R�cksprung schreiben.
	 */

	mrs r0, cpsr
	bic r0, r0, #0x1F
	orr r0, r0, #0x13	//Supervisormode
	msr spsr_hyp, r0
	eret

