
#include <kprintf.h> // Platzahlter für die Headerdatei, die euer kprintf enthält

void _check_return_jump(void);
void _check_registers(void);
void _check_cpsr(void);
void _check_spsr(void);

void register_checker(void)
{
	unsigned int cpsr;
	asm ("mrs %0, cpsr" : "=r" (cpsr));
	cpsr &= 0x1f;

	kprintf("\nBitte sicherstellen, dass waehrend der Tests Interrupts in ausreichender\n\r"
	       "Menge auftreten und auch durch euch behandelt werden!\n\r"
	       "\n\r"
	       "Die Dauer jedes Tests ist angegeben. Weicht sie stark ab, habt ihr\n\r"
	       "wahrscheinlich ein nicht durch die Tests abgedecktes Problem. (Zudem haengen\n\r"
	       "CPSR- und General-Purpose-Register-Test sehr voneinander ab: CPSR-Test braucht\n\r"
	       "Register, Register-Test brauch Flags. Also Registerdump kritisch betrachten.)\n\r"
	       "\n\r"
	       "Wenn eine Missstand festgestellt wird, wird eine Undef-Exception ausgelöst.\n\r"
	       "Mittels objdump, Quellen und Registersatz sollte sich das Problem einkreisen\n\r"
	       "lassen.\n\r"
	       "\n\r"
	       "(Alle Angaben ohne Gewaehr.)\n\n\r");

	if (cpsr == 0b10010) {
		kprintf(">>> Prozessor ist im IRQ-Modus => LR und SPSR verloren nach Interrupt\n\r"
		       ">>> (Insbesondere geht der Rücksprung aus dem ersten Test schief.)\n\r"
		       "\n\r"
		       ">>> Abbruch!\n\r");
		return;
	}

	kprintf("Pruefe richtige Ruecksprung-Adresse (ca. 5 Sekunden)\n\r");
	_check_return_jump();

	kprintf("Pruefe auf Register-Aenderungen (R0-R14, ca. 10 Sekunden)\n\r");
	_check_registers();

	if (cpsr == 0b10000) {
		kprintf("\n>>> Prozessor ist im User-Modus => Keine Pruefung auf CPSR-Aenderungen,\n\r"
		       ">>> da dort Modus-Bits geändert werden. (Wie wäre es mit dem System-Modus?)\n\n\r");
	} else {
		kprintf("Pruefe auf CPSR-Aenderungen (ca. 15 Sekunden)\n\r");
		_check_cpsr();
	}

	if (cpsr == 0b10000 || cpsr == 0b11111) {
		kprintf("\n>>> Prozessor ist im User- oder System-Modus => Keine Pruefung auf SPSR-Aenderungen,\n\r"
		       ">>> da es dieses Register nicht gibt. (Wie wäre es mit dem Supervisor-Modus?)\n\n\r");
	} else {
		kprintf("Pruefe auf SPSR-Aenderungen (ca. 15 Sekunden)\n\r");
		_check_spsr();
	}

	kprintf("Fertig! Es _scheint_ so, als wuerde es funktionieren.\n\r");
}
