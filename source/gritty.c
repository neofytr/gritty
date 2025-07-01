__asm__(
    "call main\n"
    "call exit\n");

#include <gritty.h>

extern void write(char chr);

void test(char chr)
{
    (void)chr;
    return;
}
void main()
{
    test('a');
    return;
}