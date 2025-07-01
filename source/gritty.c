__asm__(".code16gcc\n"
        "call main\n"
        "call exit\n");

#include <gritty.h>

extern void write(void);

void main()
{
    write();
    return;
}