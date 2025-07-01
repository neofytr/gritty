__asm__(
    "call main\n"
    "call exit\n");

#include <gritty.h>

extern void xputchar(uint8_t chr);

void putchar(uint8_t chr)
{
    xputchar(chr);
    return;
}

void print(uint8_t *str)
{
    if (!str)
        return;

    while (*str)
        putchar(*str++);

    return;
}
void main()
{
    print((uint8_t *)"hello world");
    return;
}