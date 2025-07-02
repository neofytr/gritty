__asm__(
    "call main\n"
    "call exit\n");

#include <gritty.h>
#include <assembly.h>
#include <shapes.h>

static void *curr_heap;
static boolean alloc_init = false;

void putchar(uint8_t chr)
{
    xputchar(chr);
    return;
}

uint8_t getchar(void)
{
    uint8_t al, ah;
    uint16_t ax;
    uint8_t ret;

    ax = (uint16_t)xgetchar();
    al = (uint8_t)(ax & 0x00ff);
    ah = (uint8_t)(ax & 0xff00) >> 8;

    ret = (al) ? al : ah;
    return ret;
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
    point_t *one, *three;

    one = mk_point(10, 10, 0);
    three = mk_point(25, 20, 0);

    video_mode(BW_GRAPHICS_MODE);
    draw_line(mk_line(one, three, 1, 0));

    return;
}
