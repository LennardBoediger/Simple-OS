ENTRY(_start)
SECTIONS
{
. = 0x00200000;
usrtext : {
    ./user/?*(.text*)
    }
usrrodata : {
    ./user/?*(.rodata*)
    }
usrdata : {
    ./user/?*(.data*)
}
usrbss : {
    ./user/?*(.bss*)
}

. = 0x00100000;
.init : {
    *(.init*)
}
.text : {
    *(.text*)
    }
.rodata : {
    *(.rodata*)
    }
.bss : {
    *(.bss)*(COMMON)
    }
.data : {
    *(.data)
    }
}