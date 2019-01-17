ENTRY(_start)
SECTIONS
{
	. = 0x00400000;
	.usrtext : { ./user/?*(.text*) }
	
	. = 0x00500000;
	.usrrodata : { ./user/?*(.rodata*) }


	. = 0x00600000;
	.usrdata : { ./user/?*(.data*) }

    . = 0x00700000;
	.usrbss : { ./user/?*(.bss*) }
	.usrCOMMON : { ./user/?*(.COMMON*) }


	. = 0x00100000;
	.init : { *(.init*) }

	. = 0x00200000;
	.text : { *(.text*) }

	. = 0x00300000;
	.data : { *(.data*) }
	.rodata : { *(.rodata*) }
	.bss : { *(.bss*) }
	.COMMON : { *(.COMMON*) }
}
