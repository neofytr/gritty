#include <bmp.h>
#include <errnum.h>
#include <gritty.h>

bmp_t *parseBMP(const char *filename, uint8_t *pixelArr, uint16_t *pixelArrSize)
{
    fileHandle_t file;
    bmp_t *bmp = NULL;
    int16_t ret;
    uint8_t *image = NULL;
    header_t *header = NULL;
    infoHeader_t *infoHeader = NULL;
    uint16_t imageSize;

    if (!filename)
    {
        errnum = ERR_INVALID_ARGS;
        if (RETURN_ACTION)
            action = ACTION_FIX_ARGS;
        goto _end;
    }

    file = openFile(filename, READ_ONLY, EVERYONE_FULL_ACCESS, INHERITABLE);
    if (file < 0)
        goto _end; // errnum and action would automatically have been set by openFile upon failure

    bmp = alloc(sizeof(bmp_t));
    if (!bmp)
        goto _end; // errnum and action would be set by alloc

    header = alloc(sizeof(header_t));
    if (!header)
        goto _end;

    infoHeader = alloc(sizeof(infoHeader_t));
    if (!infoHeader)
        goto _end;

    ret = readFile(file, sizeof(header_t), header);
    if (ret < 0)
        goto _end;

    if (!IS_BMP_SIG(bmp->header))
    {
        errnum = ERR_INVALID_BMP;
        if (RETURN_ACTION)
            action = ACTION_GIVE_VALID_BMP;

        goto _end;
    }

    ret = readFile(file, sizeof(infoHeader_t), &bmp->infoHeader);
    if (ret < 0)
        goto _end;

    if (infoHeader->compression)
    {
        errnum = ERR_INVALID_BMP;
        if (RETURN_ACTION)
            action = ACTION_GIVE_VALID_BMP;

        goto _end;
    }

    ret = readFile(file, NUM_COLORS * sizeof(color_t), bmp->colorTable);
    if (ret < 0)
        goto _end;

    imageSize = infoHeader->imageSize * sizeof(uint8_t);
    if (!imageSize)
        goto _noerr_end;

    image = alloc(infoHeader->imageSize);
    if (!image)
        goto _end;

    ret = readFile(file, imageSize, image);
    if (ret < 0)
        goto _end;

    pixelArr = image;
    *pixelArrSize = imageSize * sizeof(uint8_t);

    // each byte of image consists of data for two pixels (4-bit each)
    // for each pixel, we have a 4-bit index into the color table

    bmp->header = header;
    bmp->infoHeader = infoHeader;

_noerr_end:

    errnum = ERR_NO_ERR;
    if (RETURN_ACTION)
        action = ACTION_NO_ACTION;
_end:

    if (header)
        dealloc(header);
    if (infoHeader)
        dealloc(infoHeader);
    if (image)
        dealloc(image);
    if (file >= 0)
        closeFile(file);

    return NULL;
}