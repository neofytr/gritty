#include "buildsysdep/neobuild.h"
#include <stdio.h>

#define SRC "source/"
#define BIN "bin/"
#define INC "include/"
#define ASMINC INC "asm/"

#define MAX_BUFLEN (2048 + 1)

#define CFLAGS "-O0 -Wall -Wextra -Wpedantic " /* optimize the code moderately (level 2) */                     \
               "-std=c99 "                     /* use the c99 language standard */                              \
               "-m16 -march=i386 "             /* generate 16-bit code; target the i386 architecture */         \
               "-fno-stack-protector "         /* disable stack smashing protection */                          \
               "-ffreestanding "               /* compile without assuming standard library or startup files */ \
               "-fno-pic -fno-pie -nostdlib "                                                                   \
               "-I " INC /* add the directory in INC as an include path */

#define LFLAGS "-nmagic --script=gritty.ld -m elf_i386" // x86-64 output is default; so we change it to elf_i386
/* --nmagic disables automatic page alignment of output sections like .text and .data */
/* this makes the final binary smaller by preventing extra padding added to align sections to memory pages (typically 4kb) */
/* it also allows more precise control over layout, which is important in low-level, freestanding, or bootloader environments */
/* unlike --omagic, it keeps the text section read-only unless changed explicitly in the linker script */
/* --script=gritty.ld tells the linker to use the custom gritty.ld script to fully control memory layout, section placement, and entry point */

int main(int argc, char **argv)
{
    neocmd_t *cmd, *tmp, *as;
    char buf[MAX_BUFLEN];
    neorebuild("neo.c", argv, &argc);

    if (argc > 1 && !strcmp(argv[1], "clean"))
    {
        cmd = neocmd_create(BASH);
        neocmd_append(cmd, "rm -f " BIN "*.o " BIN "*.com " BIN "*.asm ");
        neocmd_run_sync(cmd, NULL, NULL, false);
        neocmd_delete(cmd);
        return EXIT_SUCCESS;
    }

    if (argc > 1 && !strcmp(argv[1], "push"))
    {
        if (argc < 3)
        {
            printf("Usage: %s push \"commit message\"\n", argv[0]);
            return EXIT_FAILURE;
        }

        snprintf(buf, MAX_BUFLEN - 1, "git commit -m \"%s\" &&", argv[2]);
        buf[MAX_BUFLEN - 1] = 0;

        cmd = neocmd_create(BASH);
        neocmd_append(cmd, "./neo clean");
        neocmd_run_sync(cmd, NULL, NULL, false);

        tmp = neocmd_create(BASH);
        neocmd_append(tmp, "git add . &&");
        neocmd_append(tmp, buf);
        neocmd_append(tmp, "git push -u origin main");

        neocmd_run_sync(tmp, NULL, NULL, false);
        neocmd_delete(cmd);
        neocmd_delete(tmp);
        return EXIT_SUCCESS;
    }

    neo_compile_to_object_file(GCC, SRC "gritty.c", BIN "gritty.o", CFLAGS, false);
    neo_compile_to_object_file(GCC, SRC "shapes.c", BIN "shapes.o", CFLAGS, false);

    cmd = neocmd_create(BASH);

    // assemble xgfx.asm
    neocmd_append(cmd, "nasm -i" ASMINC);
    neocmd_append(cmd, "-f elf32");
    neocmd_append(cmd, SRC "xgfx.asm");
    neocmd_append(cmd, "-o", BIN "xgfx.o");

    neocmd_run_sync(cmd, NULL, NULL, false);

    // link xgfx.o with gritty.o
    neo_link(LD, BIN "gritty.com", LFLAGS, false, BIN "gritty.o", BIN "xgfx.o", BIN "shapes.o");

    if (argc > 1 && !strcmp(argv[1], "dis"))
    {
        tmp = neocmd_create(BASH);
        neocmd_append(tmp, "objdump -D -b binary -m i8086 --disassembler-options=intel", BIN "gritty.com");
        neocmd_run_sync(tmp, NULL, NULL, false);
        neocmd_delete(tmp);
    }

    if (argc > 1 && !strcmp(argv[1], "run"))
    {
        tmp = neocmd_create(BASH);
        neocmd_append(tmp, "dosbox", BIN "gritty.com");
        neocmd_run_sync(tmp, NULL, NULL, false);
        neocmd_delete(tmp);
    }

    neocmd_delete(cmd);
    return EXIT_SUCCESS;
}
