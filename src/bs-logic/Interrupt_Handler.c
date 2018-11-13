#include "../include/kprintf.h"
#include "../include/interrupt_regs_driver.h"

/** ab hier defines für timer*/

#define TIMER_PRESCALE_MSB_SHIFT 3
#define TIMER_PRESCALE_LSB_SHIFT 2
#define WNR_BIT_SHIFT 11

void print_psr_bits(uint32_t _psr) {

    char bits[] = {'N','Z','C','V','_','I','F','T','\0'}; //0
    int i;
    for (i = 0; i <=3; ++i) {
        if (((_psr & (1 << 31-i)) >> (31-i)) == 0){
            bits[i]='_';
        }
    }
    for (i = 5; i <=7; ++i) {
        if (((_psr & (1 << 12-i)) >> (12-i)) == 0){
            bits[i]='_';
        }
    }
/*    int i;
    //NZCV-Bits
    for (i = 31; i >= 28; i--) {
        //if (((_psr & (1 << i)) >> i) == 0) {
            bits[i - 24] = '_';
        //}
    }
    //IFT-Bits
    for (i = 7; i >= 5; i--) {
       // if (((_psr & (1 << i)) >> i) == 0) {
            bits[i-5] = '_';
       // }
    }
*/    kprintf("%s", bits);
}

void print_mode_reg(){
    kprintf(">>> Aktuelle modusspezifische Register <<<\n\r");
    kprintf("             LR         SP          SPSR\n\r");
    uint32_t tmp_lr,tmp_sp, tmp_spsr;
    tmp_lr = get_lr_sys();
    tmp_sp = get_sp_sys();
    kprintf("User/System: %x %x\n\r", tmp_lr, tmp_sp);
    tmp_lr = get_lr_svc();
    tmp_sp = get_sp_svc();
    tmp_spsr = get_spsr_svc();
    kprintf("Supervisor:  %x %x %x\n\r", tmp_lr, tmp_sp, tmp_spsr);
}

void print_timerval(uint32_t cpsr) {
    kprintf("Timervalue: %i\n\r",timer_reg->VALUE);
    kprintf("CPSR: %x", cpsr);
}
void print_data_abort_reason(){
    uint32_t dfar = read_dfar();
    uint32_t dfsr = read_dfsr();
    if((dfsr & (1 << WNR_BIT_SHIFT)) >> WNR_BIT_SHIFT){
        kprintf("Zugriff: schreibend auf Adresse %x\n", dfar);
    } else{
        kprintf("Zugriff: lesend auf Adresse %x\n", dfar);
    }
}
void print_interrupt(uint32_t stackadress, uint32_t cpsr, uint32_t spsr, char* interrupt_name, int32_t pc_offset){
    kprintf("\n\r");
    int i;
    for (i = 0; i < 47; ++i) {
        kprintf("#");
    }
    //PC wird bei Ausnahme nach LR geschrieben + wegen pipline muss offset bechatet werden
    kprintf("\n\r%s an der Adresse %x\n\r",interrupt_name, *(int*) (stackadress+14*4) + pc_offset);

    //DFSR auslesen und Ausgabe mit switch-case verschönern

    kprintf(">>> Registerschnappschuss (aktueller Modus) <<<\n\r");
    for (i = 0; i <= 7; i++) {
        kprintf("R%u: %x\t", i, *(int*) (stackadress+i*4));
        kprintf("R%u: %x\n\r", i+8, *(int*) (stackadress+(i+8)*4));
    }

    kprintf(">>> Aktuelle Statusregister (SPSR im aktuellen Modus) <<<\n\r");
    kprintf("CPSR: ");
    print_psr_bits(cpsr);
//    print_psr_mode(cpsr);
    kprintf(" %x\n\r", cpsr);
    kprintf("SPSR: ");
    print_psr_bits(spsr);
//    print_psr_mode(spsr);
    kprintf(" %x\n\r", spsr);
    for (i = 0; i < 47; ++i) {
        kprintf("#");
    }
    print_mode_reg();
    kprintf("\n\r");
}


void reset(uint32_t stackadress, uint32_t cpsr, uint32_t spsr) {
    print_interrupt(stackadress, cpsr, spsr, "RESET", 0);
}

void undef(uint32_t stackadress, uint32_t cpsr, uint32_t spsr) {
    print_interrupt(stackadress, cpsr, spsr, "UNDEFINED", -4);
}

void swi(uint32_t stackadress, uint32_t cpsr, uint32_t spsr) {
    print_interrupt(stackadress, cpsr, spsr, "SOFTWARE INTERRUPT", -4);
}


void prefab(uint32_t stackadress, uint32_t cpsr, uint32_t spsr) {
    print_interrupt(stackadress, cpsr, spsr, "PREFETCH ABORT", -4);
}

void dataab(uint32_t stackadress, uint32_t cpsr, uint32_t spsr) {
    print_interrupt(stackadress, cpsr, spsr, "DATA ABORT", -8);
    print_data_abort_reason();
}

void irq(uint32_t stackadress, uint32_t cpsr, uint32_t spsr) {
    print_interrupt(stackadress, cpsr, spsr, "TIMER INTERRUPT", -8);
    recognize_irq_interrupt();
}

void fiq(uint32_t stackadress, uint32_t cpsr, uint32_t spsr) {
    print_interrupt(stackadress, cpsr, spsr, "DATA ABORT", -8);
}