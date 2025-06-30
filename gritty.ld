OUTPUT_FORMAT(raw_binary)    /* output a raw binary file with no elf or other headers */

SECTIONS
{
	. = 0x100;               /* set the location counter to 0x100 (256),
	                            this is where code will begin in the binary;
	                            in dos .com programs, the first 256 bytes are reserved for psp */

	.text :
	{
		*(.text);          /* include all input sections named .text (machine code)
		                      from object files into this output .text section */
	}

	.data :
	{
		*(.data);          /* include initialized global and static variables */
		*(.rodata);        /* include read-only data like string literals or const values */
		*(.bss);           /* include uninitialized data (zero-initialized at runtime),
		                      this is usually not written into the binary unless explicitly placed */
	}

	heap = ALIGN(2);        /* define a symbol 'heap' at the next 2-byte aligned address,
	                           useful to mark where heap memory can begin at runtime */
}
