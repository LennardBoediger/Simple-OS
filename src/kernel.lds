ENTRY(_start)
SECTIONS
{
. = 0x00400000;
.usrtext : {
    ./user/?*(.text)
    ./syscalls/?*(.text)
    }
. = 0x00500000;
.usrdata : {
    ./user/?*(.data*)
    ./syscalls/?*(.data)
}
.usrrodata : {
    ./user/?*(.rodata*)
    ./syscalls/?*(.rodata)
    }
.usrbss : {
    ./user/?*(.bss)
    ./syscalls/?*(.bss)
}

. = 0x00100000;
.init : {
    *(.init)
    }

. = 0x00200000;
.text : {
    *(.text*)
    }

. = 0x00300000;
.data : {
    *(.data)
    }
.rodata : {
    *(.rodata*)
    }
.bss : {
    *(.bss)
    }
}