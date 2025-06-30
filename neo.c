#include "buildsysdep/neobuild.h"
#include <stdio.h>

#define SRC "source/"
#define BIN "bin/"
#define INC "include/"

#define CFLAGS "-O2 -Wall -Wextra -Wpedantic -Werror " /* optimize the code moderately (level 2) */                     \
               "-std=c99 "                             /* use the c99 language standard */                              \
               "-m16 --march=i386 "                    /* generate 16-bit code; target the i386 architecture */         \
               "-fno-stack-protector "                 /* disable stack smashing protection */                          \
               "-ffreestanding "                       /* compile without assuming standard library or startup files */ \
               "-I " INC                               /* add the directory in INC as an include path */

#define LFLAGS "--nmagic --script=gritty.ld"
/* --nmagic disables automatic page alignment of output sections like .text and .data */
/* this makes the final binary smaller by preventing extra padding added to align sections to memory pages (typically 4kb) */
/* it also allows more precise control over layout, which is important in low-level, freestanding, or bootloader environments */
/* unlike --omagic, it keeps the text section read-only unless changed explicitly in the linker script */
/* --script=gritty.ld tells the linker to use the custom gritty.ld script to fully control memory layout, section placement, and entry point */

int main(int argc, char **argv)
{
    neorebuild("neo.c", argv, &argc);

    neo_compile_to_object_file(GCC, SRC "gritty.c", NULL, CFLAGS, false);
    neo_link(GCC, BIN "gritty", NULL, false, SRC "main.o");
    return EXIT_SUCCESS;
}
