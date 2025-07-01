OUTPUT_FORMAT(binary)    /* output a raw binary file with no elf or other headers */

/* the order of .text sections here follows the order of object files
   passed to the linker on the command line (e.g. a.o, then b.o, etc);
   to control section order explicitly, use *(file.o(.text)) */

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

	__heap = ALIGN(2);        /* define a symbol '__heap' at the next 2-byte aligned address,
	                           useful to mark where heap memory can begin at runtime */

	/* a symbol is a label for a memory address */
	/* C interprets a symbol in the way it's declared */
	/* extern char __heap */
	/* Now, heap contains the byte at the memory the symbol pointed to (automatic dereferencing of symbols according to their declaration) */
	/* to get the intended address, use &__heap */
	/* the memory address that the symbol __heap points to can be accessed as &__heap inside C code
	and as __heap inside assembly code */
}
