#define INSIDE_GRITTY

__asm__(
    "call main\n"
    "call exit\n");

#include <gritty.h>
#include <assembly.h>
#include <shapes.h>
#include <errnum.h>
#include <stdarg.h>

#define PRINTF_BUF_LEN (2048 + 1) // 1 for null-byte

static void *curr_heap;
static boolean alloc_init = false;

static int16_t vprintFormattedToBuffer(uint8_t *buf, uint16_t max_len, const char *format, va_list args);
static int16_t vprintFormattedToFile(fileHandle_t fileHandle, const char *format, va_list args);

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

static int16_t vprintFormattedToBuffer(uint8_t *buf, uint16_t max_len, const char *format, va_list args)
{
    uint8_t numBuf[32] = {0};
    uint16_t numIndex;
    boolean isNeg;
    int32_t num;
    uint32_t unum;
    uint8_t *ptr;
    uint16_t len;
    int16_t tmp;
    uint16_t bufIndex, formatIndex;
    const char *str;
    uint8_t *strPtr;
    uint16_t strLen;
    uint8_t ch;
    uint8_t base;
    const char *hexChars = "0123456789abcdef";

    if (!format)
    {
        errnum = ERR_INVALID_ARGS;
        if (RETURN_ACTION)
            action = ACTION_FIX_ARGS;
        return -1;
    }

    ptr = (uint8_t *)format;
    len = 0;

    while (*ptr++)
        len++;

    bufIndex = formatIndex = 0;
    while (bufIndex < max_len - 1)
    {
        if (formatIndex >= len)
            break;

        if (format[formatIndex] == '%')
        {
            if (formatIndex + 1 >= len)
            {
                buf[bufIndex++] = '%';
                formatIndex++;
                break;
            }
            else if (format[formatIndex + 1] == '%')
            {
                formatIndex += 2;
                buf[bufIndex++] = '%';
            }
            else if (format[formatIndex + 1] == 'd')
            {
                formatIndex += 2;
                num = va_arg(args, int32_t);

                numIndex = 0;
                isNeg = num < 0;
                if (isNeg)
                    num = -num;

                if (!num)
                {
                    numBuf[numIndex++] = '0';
                }
                else
                {
                    while (num)
                    {
                        numBuf[numIndex++] = num % 10 + '0';
                        num /= 10;
                    }
                }

                if (isNeg)
                    numBuf[numIndex++] = '-';

                for (tmp = numIndex - 1; tmp >= 0; tmp--)
                {
                    if (bufIndex >= max_len - 1)
                        break;
                    buf[bufIndex++] = numBuf[tmp];
                }
            }
            else if (format[formatIndex + 1] == 'u')
            {
                formatIndex += 2;
                unum = va_arg(args, uint32_t);

                numIndex = 0;
                if (!unum)
                {
                    numBuf[numIndex++] = '0';
                }
                else
                {
                    while (unum)
                    {
                        numBuf[numIndex++] = unum % 10 + '0';
                        unum /= 10;
                    }
                }

                for (tmp = numIndex - 1; tmp >= 0; tmp--)
                {
                    if (bufIndex >= max_len - 1)
                        break;
                    buf[bufIndex++] = numBuf[tmp];
                }
            }
            else if (format[formatIndex + 1] == 'x')
            {
                formatIndex += 2;
                unum = va_arg(args, uint32_t);

                numIndex = 0;
                if (!unum)
                {
                    numBuf[numIndex++] = '0';
                }
                else
                {
                    while (unum)
                    {
                        numBuf[numIndex++] = hexChars[unum % 16];
                        unum /= 16;
                    }
                }

                for (tmp = numIndex - 1; tmp >= 0; tmp--)
                {
                    if (bufIndex >= max_len - 1)
                        break;
                    buf[bufIndex++] = numBuf[tmp];
                }
            }
            else if (format[formatIndex + 1] == 'X')
            {
                formatIndex += 2;
                unum = va_arg(args, uint32_t);

                numIndex = 0;
                if (!unum)
                {
                    numBuf[numIndex++] = '0';
                }
                else
                {
                    while (unum)
                    {
                        ch = hexChars[unum % 16];
                        if (ch >= 'a' && ch <= 'f')
                            ch = ch - 'a' + 'A';
                        numBuf[numIndex++] = ch;
                        unum /= 16;
                    }
                }

                for (tmp = numIndex - 1; tmp >= 0; tmp--)
                {
                    if (bufIndex >= max_len - 1)
                        break;
                    buf[bufIndex++] = numBuf[tmp];
                }
            }
            else if (format[formatIndex + 1] == 'c')
            {
                formatIndex += 2;
                ch = (uint8_t)va_arg(args, int32_t);
                if (bufIndex < max_len - 1)
                    buf[bufIndex++] = ch;
            }
            else if (format[formatIndex + 1] == 's')
            {
                formatIndex += 2;
                str = va_arg(args, const char *);
                if (str)
                {
                    strPtr = (uint8_t *)str;
                    while (*strPtr && bufIndex < max_len - 1)
                    {
                        buf[bufIndex++] = *strPtr++;
                    }
                }
                else
                {
                    const char *nullStr = "(null)";
                    strPtr = (uint8_t *)nullStr;
                    while (*strPtr && bufIndex < max_len - 1)
                    {
                        buf[bufIndex++] = *strPtr++;
                    }
                }
            }
            else if (format[formatIndex + 1] == 'p')
            {
                formatIndex += 2;
                unum = (uint32_t)va_arg(args, void *);

                if (bufIndex < max_len - 3)
                {
                    buf[bufIndex++] = '0';
                    buf[bufIndex++] = 'x';
                }

                numIndex = 0;
                if (!unum)
                {
                    numBuf[numIndex++] = '0';
                }
                else
                {
                    while (unum)
                    {
                        numBuf[numIndex++] = hexChars[unum % 16];
                        unum /= 16;
                    }
                }

                for (tmp = numIndex - 1; tmp >= 0; tmp--)
                {
                    if (bufIndex >= max_len - 1)
                        break;
                    buf[bufIndex++] = numBuf[tmp];
                }
            }
            else
            {
                buf[bufIndex++] = format[formatIndex++];
            }
        }
        else
        {
            buf[bufIndex++] = format[formatIndex++];
        }
    }

    buf[bufIndex] = '\0';
    errnum = ERR_NO_ERR;
    if (RETURN_ACTION)
        action = ACTION_NO_ACTION;
    return bufIndex;
}

int16_t printFormattedToBuffer(uint8_t *buf, uint16_t max_len, const char *format, ...)
{
    va_list args;
    int16_t ret;

    va_start(args, format);
    ret = vprintFormattedToBuffer(buf, max_len, format, args);
    va_end(args);
    return ret;
}

int16_t printFormatted(const char *format, ...)
{
    va_list args;
    int16_t ret;

    va_start(args, format);
    ret = vprintFormattedToFile(OUTPUT, format, args);
    va_end(args);

    return ret;
}

static int16_t vprintFormattedToFile(fileHandle_t fileHandle, const char *format, va_list args)
{
    uint8_t buf[PRINTF_BUF_LEN];
    int16_t ret;
    int16_t len;

    // this call can't fail
    len = vprintFormattedToBuffer(buf, PRINTF_BUF_LEN, format, args);

    ret = writeFile(fileHandle, len, buf);
    return ret;
}

int16_t printFormattedToFile(fileHandle_t fileHandle, const char *format, ...)
{
    va_list args;
    int16_t len;

    va_start(args, format);
    // this call can't fail
    len = vprintFormattedToFile(fileHandle, format, args);
    va_end(args);

    return len;
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
    const char *str = "hello";
    printFormatted("hello %s", str);
    return;
}
