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

int16_t print(const char *str)
{
    const uint8_t *ptr;
    if (!str)
    {
        errnum = ERR_INVALID_ARGS;
        if (RETURN_ACTION)
            action = ACTION_FIX_ARGS;
        return -1;
    }

    ptr = (uint8_t *)str;
    uint16_t len = 0;
    while (*ptr++)
        len++;

    return writeFile(OUTPUT, len, (uint8_t *)str);
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

    reserved = (uint8_t)~(1 << 3);
    mode = (uint8_t)(accessMode & reserved) | (uint8_t)(sharingMode << 4) | (uint8_t)(inheritenceMode << 7);

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
    if (RETURN_ACTION)
        action = ACTION_NO_ACTION;
    return file;
}

boolean closeFile(fileHandle_t file)
{
    int16_t ret;
    uint16_t err, act;

    ret = xclose_file(file);
    if (ret < 0)
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
            errnum = -ret;

        return false;
    }

    errnum = ERR_NO_ERR;
    if (RETURN_ACTION)
        action = ACTION_NO_ACTION;
    return true;
}

int16_t readFile(fileHandle_t fileHandle, uint16_t bytes, uint8_t *buffer)
{
    int16_t ret;
    uint16_t err, act;
    if (!buffer)
    {
        errnum = ERR_INVALID_ARGS;
        if (RETURN_ACTION)
            action = ACTION_FIX_ARGS;
        return -1;
    }

    ret = xread_file(fileHandle, bytes, buffer);
    if (ret < 0)
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
            errnum = -ret;

        return -1;
    }

    errnum = ERR_NO_ERR;
    if (RETURN_ACTION)
        action = ACTION_NO_ACTION;
    return ret;
}

int16_t writeFile(fileHandle_t fileHandle, uint16_t bytes, uint8_t *buffer)
{
    int16_t ret;
    uint16_t err, act;
    if (!buffer)
    {
        errnum = ERR_INVALID_ARGS;
        if (RETURN_ACTION)
            action = ACTION_FIX_ARGS;
        return -1;
    }

    ret = xwrite_file(fileHandle, bytes, buffer);
    if (ret < 0)
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
            errnum = -ret;

        return -1;
    }

    errnum = ERR_NO_ERR;
    if (RETURN_ACTION)
        action = ACTION_NO_ACTION;
    return ret;
}

void main()
{
    print("hello");
    return;
}
