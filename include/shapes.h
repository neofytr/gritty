#ifndef SHAPES_H
#define SHAPES_H

#include <base.h>

#define BW_TEXT_MODE 0
#define BW_TEXT_MAX_X 40
#define BW_TEXT_MAX_Y 25

#define MAX_MODE 13

// the color field does nothing in the black and white text mode
typedef struct packed
{
    uint16_t x, y;
    uint8_t color : 4; // our color pallette supports c0 to cfff (16 colors), so we need only 4 bits
} point_t;

// the color field does nothing in the B/W Text mode
typedef struct packed
{
    point_t *one, *two;
    uint16_t thickness;
    uint8_t color : 4;
} line_t;

void video_mode(uint8_t mode);
boolean draw_point(point_t *point);
point_t *mk_point(uint16_t x, uint16_t y, uint8_t color);
line_t *mk_line(point_t *one, point_t *two, uint16_t thickness, uint8_t color);
boolean draw_line(line_t *line);

#endif