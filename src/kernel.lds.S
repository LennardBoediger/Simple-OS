ENTRY(_start)
SECTIONS
{
    . = 0x00400000
    .user_text : {
        ./user/?*(.text*)
        syscalls/?*(.text*)
        }
    . = 0x00500000
    .usrrodata : {
        user/?*(.rodata*)
        syscalls/?*(.rodata*)
        }

    . = 0x00600000
    .usrdata : {
        user/?*(.data)
        syscalls/?*(.data)
    }
    VAR1 =.
    .usrbss : {
        user/?*(.bss)
        syscalls/?*(.bss)
    }
    VAR2 =.

    . = 0x00100000
    .init : { *(.init) }

    . = 0x00200000
    .text : { *(.text*) }

    . = 0x00300000
    .data : { *(.data) }
    .rodata : { *(.rodata*) }
    .bss : { *(.bss) }
}
