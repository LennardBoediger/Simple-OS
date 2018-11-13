#include "../include/uart_driver.h"
#include "../include/kprintf.h"
#include "../include/Interrupt_Handler.h"
#include "../include/dataab_helper.h"



void force_interrupts (char c) {
    switch (c) {
        case 's':
            asm("swi 99");
            break;
        case 'a':
            force_dataab();
            break;
        case 'u':
            asm("udf");
            break;
    }
}

void inputtest(){
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



    while (1){
        char c = uart_receive();
        if (c) {
            kprintf("you pressed: %c\n\r", c);
            force_interrupts(c);
        }
    }
}