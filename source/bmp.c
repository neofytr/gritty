#include <bmp.h>
#include <errnum.h>
#include <gritty.h>

static int8_t closestSCGColor(color_t *color)
{
    uint8_t red, green, blue;
    int32_t minDistance;
    int8_t closest;
    int32_t dr, dg, db;
    int32_t distance;
    if (!color)
    {
        return -1;
    }

    red = color->red;
    green = color->green;
    blue = color->blue;

    closest = 0;
    minDistance = INT32_MAX;

    for (int8_t i = 0; i < NUM_COLORS; ++i)
    {
        dr = red - scgPalette[i].red;
        dg = green - scgPalette[i].green;
        db = blue - scgPalette[i].blue;
        distance = dr * dr + dg * dg + db * db; // no need to sqrt for comparison

        if (distance < minDistance)
        {
            minDistance = distance;
            closest = i;
        }
    }
    return closest;
}

void freeBMP(bmp_t *bmp)
{
    if (!bmp)
    {
        errnum = ERR_INVALID_ARGS;
        if (RETURN_ACTION)
            action = ACTION_FIX_ARGS;
        return;
    }

    dealloc(bmp->header);
    dealloc(bmp->infoHeader);
    dealloc(bmp->pixelArr);
    dealloc(bmp);

    return;
}

boolean renderBMP(bmp_t *bmp)
{
    color_t *colorTable;
    uint8_t *pixelArr;
    uint16_t pixelArrLen;
    uint16_t rows, cols;
    uint16_t width;
    uint16_t pixelID, tmp;
    uint16_t colorID;
    point_t point;
    color_t color;
    if (!bmp)
    {
        errnum = ERR_INVALID_ARGS;
        if (RETURN_ACTION)
            action = ACTION_FIX_ARGS;
        return;
    }

    colorTable = bmp->colorTable;
    pixelArr = bmp->pixelArr;
    pixelArrLen = bmp->pixelArrLen;

    rows = bmp->infoHeader->height;
    cols = bmp->infoHeader->width;
    width = cols;

    // each scan line is padded to a 4-byte boundary
    cols = (cols + 3) & (~3);

    if (!colorTable || !pixelArr || !pixelArrLen)
    {
        errnum = ERR_INVALID_ARGS;
        if (RETURN_ACTION)
            action = ACTION_FIX_ARGS;
        return;
    }

    // scan lines are stored bottom to top
    for (uint16_t row = 0; row < rows; row++)
    {
        for (uint16_t col = 0; col < width; col++)
        {
            tmp = row * cols + col;
            pixelID = tmp / 2;

            // the higher byte consists of the first byte in the pair
            // the lower order byte is the second byte in the pair
            colorID = (tmp & 0) ? (pixelArr[pixelID] >> 4) & 0x0F : pixelArr[pixelID] & 0x0F;
            color = colorTable[colorID];
            point.x = row;
            point.y = col;
            point.color = closestSCGColor(&color);
            if (!draw_point(&point))
            {
            }
        }
    }
}

bmp_t *parseBMP(const char *filename)
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

    // each byte of image consists of data for two pixels (4-bit each)
    // for each pixel, we have a 4-bit index into the color table

    bmp->header = header;
    bmp->infoHeader = infoHeader;
    bmp->pixelArr = image;
    bmp->pixelArrLen = imageSize;

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