__asm__(
    "call main\n"
    "call exit\n");

#include <gritty.h>
#include <assembly.h>

static void *curr_heap;
static bool alloc_init = false;

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
    void *ptr;
    if (!alloc_init)
    {
        alloc_init = true;
        curr_heap = &__heap;
    }

    if (!size)
        return NULL;

    ptr = curr_heap;
    curr_heap = (uint8_t *)curr_heap + size;
    return ptr;
}

void freeall(void)
{
    curr_heap = &__heap;
    return;
}
void main()
{
    uint8_t *ptr;

    ptr = alloc(5);
    if (!ptr)
    {
        print((uint8_t *)"alloc error");
        return;
    }

    ptr[0] = 'a';
    ptr[1] = 'b';
    ptr[2] = 'c';
    ptr[3] = 'd';
    ptr[4] = 0; // null byte

    print(ptr);
    return;
}
