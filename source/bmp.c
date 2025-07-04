#include <bmp.h>
#include <errnum.h>
#include <gritty.h>

bmp_t *parseBMP(const char *filename)
{
    fileHandle_t file;
    bmp_t *bmp;
    if (!filename)
    {
        errnum = ERR_INVALID_ARGS;
        if (RETURN_ACTION)
            action = ACTION_FIX_ARGS;
        return NULL;
    }

    file = openFile(filename, READ_ONLY, EVERYONE_FULL_ACCESS, INHERITABLE);
    if (file < 0)
        return NULL; // errnum and action would automatically have been set by openFile upon failure

    bmp = alloc(sizeof(bmp_t));
    if (!bmp)
        return NULL; // errnum and action would be set by alloc

    bmp->header = alloc(sizeof(header_t));
    if (!bmp->header)
    {
        dealloc(bmp);
        return NULL;
    }

    bmp->infoHeader = alloc(sizeof(infoHeader_t));
    if (!bmp->infoHeader)
    {
        dealloc(bmp->header);
        dealloc(bmp);
        return NULL;
    } 

    
}