ENTRY(_start)
SECTIONS
{
. = 0x00200000;
ersttext : {
    user/?*(.text*)
    }
erstrodata : {
    user/?*(.rodata*)
    }
. = 0x00800000;
.text : {
    *(.text*)
    }
.rodata : {
    *(.rodata*)
    }
. = 0x00A00000;
.bss : {
    *(.bss)*(COMMON)
    }
.data : {
    *(.data)
    }
}
