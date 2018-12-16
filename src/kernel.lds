ENTRY(_start)
SECTIONS
{
    //TODO: LÄUFT NICHT...
    . = 0x00200000;
    usrtxt : { user/?*(.text*) }
    usrdata : { user/?*(.data*) }
    usrrodata : { user/?*(.rodata*) }
    usrbss : { user/?*(.bss*) }
	. = 0x00100000;
	.init : { *(.init*) }
	.text : { *(.text*) }
	.data : { *(.data) }
}
