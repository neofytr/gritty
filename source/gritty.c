__asm__(
    "call main\n"
    "call exit\n");

#include <gritty.h>
void main()
{
    write('a');
    return;
}