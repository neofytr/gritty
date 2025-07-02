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
    rectangle_t *rect;
    video_mode(BWG_MODE);

    one = mk_point(100, 100, 10);
    three = mk_point(200, 200, 10);

    rect = mk_rect(one, three, 5, 10, 10, false);
    draw_rect(rect);

    xdraw_point_bwg(100, 100);
    xdraw_point_bwg(101, 101);
    xdraw_point_bwg(102, 102);
    xdraw_point_bwg(103, 103);
    xdraw_point_bwg(104, 104);
    xdraw_point_bwg(105, 105);
    xdraw_point_bwg(106, 106);
    xdraw_point_bwg(107, 107);

    return;
}
