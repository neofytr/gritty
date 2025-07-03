#define INSIDE_GRITTY

__asm__(
    "call main\n"
    "call exit\n");

#include <gritty.h>
#include <assembly.h>
#include <shapes.h>
#include <errnum.h>

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

fileHandle_t openFile(const char *filename, uint8_t accessMode, uint8_t sharingMode, uint8_t inheritenceMode)
{
    uint8_t mode, reserved;
    fileHandle_t file;
    uint16_t err, act;
    if (!filename)
    {
        errnum = ERR_INVALID_ARGS;
        if (RETURN_ACTION)
            action = ACTION_FIX_ARGS;
        return -1;
    }

    reserved = ~(1U << 3);
    mode = (accessMode & reserved) | (sharingMode << 4) | (inheritenceMode << 7);

    file = xopen_file(filename, mode);
    if (file < 0) // returns negative of the error code
    {
        if (RETURN_EXTENDED_ERRORS || RETURN_ACTION)
        {
            xget_more_err_info(&err, &act);
            if (RETURN_EXTENDED_ERRORS)
                errnum = err;
            if (RETURN_ACTION)
                action = act;
        }
        else
            errnum = -file;

        return -1;
    }

    errnum = ERR_NO_ERR;
    return file;
}

void main()
{
    video_mode(BWT_MODE);
    return;
}
