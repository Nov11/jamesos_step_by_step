ENTRY(start)
SECTIONS
{
	.text 0x100000:
	{
		code = .;
		boot.o(.text);
		*(.text);
		. = ALIGN(4096);
	}

	.data :
	{
		data = .;
		*(.data)
		*(.rodata)
		. = ALIGN(4096);
	}

	.bss :
	{
		bss = .;
		*(.bss)
		. = ALIGN(4096);
	}

	end = .;
}
