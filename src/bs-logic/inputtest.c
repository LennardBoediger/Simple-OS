#include "../include/uart_driver.h"
#include "../include/kprintf.h"
#include "../include/Interrupt_Handler.h"
#include "../include/dataab_helper.h"
#include "../include/regcheck.h"
#include "../include/interrupt_regs_driver.h"


void inputtest(){
    kprintf("Press S for Software Interrupt \n\r");
    kprintf("Press A for Data Abort \n\r");
    kprintf("Press U for Undefined Instruction \n\r");
    kprintf("Press C for Register Checker  \n\r");
    kprintf("Press E for interaktive Unterprogramm \n\r");
    kprintf("Press D to toggle IRQ-Debug Mode \n\r");

    while (1){
        char c = uart_receive();
        if (c) {
            kprintf("you pressed: %c\n\r", c);
            force_interrupts(c);
        }

    }
}