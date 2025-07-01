__asm__(
    "call main\n"
    "call exit\n");

#include <gritty.h>
#include <assembly.h>

static uint8_t *currheap;
static bool allocinit = false;

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

void *alloc(uint16_t size)
{
    uint8_t *ptr;
    if (!allocinit)
    {
        allocinit = true;
        currheap = heap;
    }

    if (!size)
        return NULL;

    ptr = currheap;
    currheap += (size);
    return ptr;
}

void freeall(void)
{
    currheap = heap;
    return;
}
void main()
{
    print((uint8_t *)"hello world");
    return;
}
