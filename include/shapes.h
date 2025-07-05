#ifndef SHAPES_H
#define SHAPES_H

#include <base.h>

#define BWT_MODE 0
#define BWT_MAX_X 40
#define BWT_MAX_Y 25

// 16 colors supported (from 0 to 15)
/*

In DOS 16-color graphics modes (like CGA/EGA/VGA 16-color modes), the colors are typically based on a 4-bit color value (from 0 to 15), where each bit corresponds to a specific RGB intensity:

Bit 0: Blue

Bit 1: Green

Bit 2: Red

Bit 3: Intensity (bright)

*/
#define SCG_MODE 0x12
#define SCG_MAX_X 640
#define SCG_MAX_Y 480

#define COLOR_BLACK 0
#define COLOR_BLUE 1
#define COLOR_GREEN 2
#define COLOR_CYAN 3
#define COLOR_RED 4
#define COLOR_MAGENTA 5
#define COLOR_BROWN 6
#define COLOR_LIGHT_GRAY 7
#define COLOR_DARK_GRAY 8
#define COLOR_LIGHT_BLUE 9
#define COLOR_LIGHT_GREEN 10
#define COLOR_LIGHT_CYAN 11
#define COLOR_LIGHT_RED 12
#define COLOR_LIGHT_MAGENTA 13
#define COLOR_YELLOW 14
#define COLOR_WHITE 15

#define MAX_MODE 0x13

typedef struct packed
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} rgb_t;

// the color field does nothing in the black and white text mode
typedef struct packed
{
    uint16_t x, y;
    uint8_t color;
} point_t;

// the color field does nothing in the B/W Text mode
typedef struct packed
{
    point_t *one, *two;
    uint16_t thickness;
    uint8_t color;
} line_t;

// color field is ignored for B/W text mode
typedef struct packed
{
    point_t *one, *three; // two diagonally opposite points
    uint16_t thickness;   // thickness of the rectangle lines
    uint8_t fg_color;     // color inside the rectangle
    uint8_t bg_color;     // color of the rectangle lines
    boolean filled;       // whether the rectangle is filled or not
} rectangle_t;

#ifdef INSIDE_GRITTY
rgb_t scgPalette[NUM_COLORS] = {
    {0, 0, 0},       // Black
    {0, 0, 170},     // Blue
    {0, 170, 0},     // Green
    {0, 170, 170},   // Cyan
    {170, 0, 0},     // Red
    {170, 0, 170},   // Magenta
    {170, 85, 0},    // Brown
    {170, 170, 170}, // Light Gray
    {85, 85, 85},    // Dark Gray
    {85, 85, 255},   // Light Blue
    {85, 255, 85},   // Light Green
    {85, 255, 255},  // Light Cyan
    {255, 85, 85},   // Light Red
    {255, 85, 255},  // Light Magenta
    {255, 255, 85},  // Yellow
    {255, 255, 255}  // White
};
#else
extern rgb_t *scgPalette;
#endif

void video_mode(uint8_t mode);        // set the current video mode
boolean draw_point(point_t *point);   // draw a point on the screen
boolean draw_line(line_t *line);      // draw a line on the screen
boolean draw_rect(rectangle_t *rect); // draw a rectangle on the screen

// shape constructors
point_t *mk_point(uint16_t x, uint16_t y, uint8_t color);
line_t *mk_line(point_t *one, point_t *two, uint16_t thickness, uint8_t color);
rectangle_t *mk_rect(point_t *one, point_t *three, uint16_t thickness, uint8_t fg_color, uint8_t bg_color, boolean filled);

#endif