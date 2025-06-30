#include "buildsysdep/neobuild.h"
#include <stdio.h>

#define SRC "source/"
#define BIN "bin/"
#define INC "include/"

#define CFLAGS "-O2 -Wall -Wextra -Wpedantic -Werror -std=c2x" \
               " -I " INC

int main(int argc, char **argv)
{
    neorebuild("neo.c", argv, &argc);

    neo_compile_to_object_file(GCC, SRC "main.c", NULL, CFLAGS, false);
    neo_link(GCC, BIN "main", NULL, false, SRC "main.o");
    return EXIT_SUCCESS;
}
