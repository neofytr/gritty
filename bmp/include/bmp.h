#ifndef __BMP_H__
#define __BMP_H__

#include <base.h>

// This library is for parsing 4-bit color depth, uncompressed BMP files
#define NUM_COLORS 16

// infoHeader_t.compression can be any of the following
#define BI_RGB 0  // no compression
#define BI_RLE8 1 // 8 bit RLE compression
#define BI_RLE4 2 // 4 bit RLE compression

#define IS_BMP_SIG(header) ((uint8_t)((header)->signature & 0xFF) == 'B') && ((uint8_t)(((header)->signature >> 8) & 0xFF) == 'M')

typedef struct packed
{
    uint16_t signature;  // 'BM'
    uint32_t fileSize;   // file size in bytes
    uint32_t reserved;   // 0
    uint32_t dataOffset; // offset from beginning of file to beginning of the bitmap data
} header_t;

typedef struct packed
{
    uint32_t size;         // size of InfoHeader (40)
    uint32_t width;        // horizontal width of bitmap in pixels
    uint32_t height;       // vertical height of bitmap in pixels
    uint16_t planes;       // number of planes (1)
    uint16_t bitsPerPixel; // this can be 1 (monochrome pallete), 4 (16 color pallete), 8 (256 color pallete), 16 (256 color pallete) or 24 (24 bit color depth)
    uint32_t compression;
    uint32_t imageSize;       // (compressed) size of image; can be 0 if compression is 0
    uint32_t xPixelsPerM;     // horizontal resolution (pixels / metre)
    uint32_t yPixelsPerM;     // vertical resolution (pixels / metre)
    uint32_t colorsUsed;      // number of colors actually used
    uint32_t importantColors; // number of important colors (0 = all)
} infoHeader_t;

typedef struct packed
{
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t reserved; // unused (=0)
} color_t;

typedef struct packed
{
    header_t *header;
    infoHeader_t *infoHeader;
    color_t colorTable[NUM_COLORS];
    uint8_t *pixelArr;
    uint16_t pixelArrLen;
} bmp_t;

// returns NULL on error (errnum and action set as required)
bmp_t *parseBMP(const char *filename);
boolean renderBMP(bmp_t *bmp);
void freeBMP(bmp_t *bmp);

#endif // __BMP_H__