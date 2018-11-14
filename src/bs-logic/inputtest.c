#include "../include/uart_driver.h"
#include "../include/kprintf.h"
#include "../include/Interrupt_Handler.h"
#include "../include/dataab_helper.h"
#include "../include/regcheck.h"


void test_kprintf(){
    kprintf("Testbegin\n\r");
    kprintf("%%c: b wird ausgegeben: %c\n\r", 'b');
    kprintf("%%s: Die Welt %s", "ist schön!\n\r");
    kprintf("%%x: 47096 hexadezimal gleich %x\n\r", 47096);
    kprintf("%%i: kleinste negative Zahl: %i \n\r", -2147483648);
    kprintf("%%i: groesste positive Zahl: %i\n\r", 2147483647);
    kprintf("%%u: groesste unsigned Int: %u\n\r", 4294967295);
    char a = '4';
    kprintf("%%p: char a befindet sich hier: %p\n\r", &a);
    kprintf("Jetzt freie Eingabe möglich:\n\r");
}

void inputtest(){
   kprintf("Press S for Software Interrupt \n\r");
   kprintf("Press D for Data Abort \n\r");
   kprintf("Press U for Undefined Instruction \n\r");
   kprintf("Press C for Register Checker  \n\r");
   kprintf("Press E for interaktive Unterprogramm \n\r");
   //kprintf("Press D to toggle IRQ-Debug Mode \n\r");

    while (1){


    }
}