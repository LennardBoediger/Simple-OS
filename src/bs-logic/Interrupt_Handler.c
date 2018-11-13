#include "../include/kprintf.h"
#include "../include/interrupt_regs_driver.h"
#include "../include/mode_reg_helper.h"
#include "../include/dataab_helper.h"

/** ab hier defines für timer*/

#define TIMER_PRESCALE_MSB_SHIFT 3
#define TIMER_PRESCALE_LSB_SHIFT 2
#define WNR_BIT_SHIFT 11
void print_psr_mode(uint32_t _psr){
    switch (_psr % 32){
        case 16:
            kprintf(" User");
            break;
        case 17:
            kprintf(" FIQ");
            break;
        case 18:
            kprintf(" IRQ");
            break;
        case 19:
            kprintf(" Supervisor");
            break;
        case 23:
            kprintf(" Abort");
            break;
        case 27:
            kprintf(" Undefined");
            break;
        case 31:
            kprintf(" System");
            break;
        default:
            kprintf(" INVALID MODE!!!!!");
            break;

    }
}
void print_psr_bits(uint32_t _psr) {

    char bits[] = {'N','Z','C','V','_','I','F','T','\0'}; //0
    int i;
    for (i = 0; i <=3; ++i) {
        if (((_psr & (1 << (31-i))) >> (31-i)) == 0){
            bits[i]='_';
        }
    }
    for (i = 5; i <=7; ++i) {
        if (((_psr & (1 << (12-i))) >> (12-i)) == 0){
            bits[i]='_';
        }
    }
    kprintf("%s", bits);
}

void print_mode_reg(){
    kprintf("\n\r>>> Aktuelle modusspezifische Register <<<\n\r");
    kprintf("             LR         SP         SPSR\n\r");
    uint32_t tmp_lr,tmp_sp;
    tmp_lr = get_lr_system();
    tmp_sp = get_sp_system();
    kprintf("User/System: %x %x\n\r", tmp_lr, tmp_sp);
    tmp_lr = get_lr_supervisor();
    tmp_sp = get_sp_supervisor();
    kprintf("Supervisor:  %x %x ", tmp_lr, tmp_sp);
    print_psr_bits(get_spsr_supervisor());
    print_psr_mode(get_spsr_supervisor());
    kprintf("\n\r");
    tmp_lr = get_lr_abort();
    tmp_sp = get_sp_abort();
    kprintf("Abort:       %x %x ", tmp_lr, tmp_sp);
    print_psr_bits(get_spsr_abort());
    print_psr_mode(get_spsr_abort());
    kprintf("\n\r");
    tmp_lr = get_lr_FIQ();
    tmp_sp = get_sp_FIQ();
    kprintf("FIQ:         %x %x ", tmp_lr, tmp_sp);
    print_psr_bits(get_spsr_FIQ());
    print_psr_mode(get_spsr_FIQ());
    kprintf("\n\r");
    tmp_lr = get_lr_IRQ();
    tmp_sp = get_sp_IRQ();
    kprintf("IRQ:         %x %x ", tmp_lr, tmp_sp);
    print_psr_bits(get_spsr_IRQ());
    print_psr_mode(get_spsr_IRQ());
    kprintf("\n\r");
    tmp_lr = get_lr_undefined();
    tmp_sp = get_sp_undefined();
    kprintf("Undefined:   %x %x ", tmp_lr, tmp_sp);
    print_psr_bits(get_spsr_undefined());
    print_psr_mode(get_spsr_undefined());
    kprintf("\n\r");
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
        kprintf("R%u: %x\t    ", i, *(int*) (stackadress+i*4));
        kprintf("R%u: %x\n\r", i+8, *(int*) (stackadress+(i+8)*4));
    }

    kprintf("\n\r>>> Aktuelle Statusregister (SPSR im aktuellen Modus) <<<\n\r");
    kprintf("CPSR: ");
    print_psr_bits(cpsr);
    print_psr_mode(cpsr);
    kprintf(" %x\n\r", cpsr);
    kprintf("SPSR: ");
    print_psr_bits(spsr);
    print_psr_mode(spsr);
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