#include <shapes.h>
#include <assembly.h>
#include <gritty.h>

static uint16_t max_x, max_y;
static boolean is_mode_set = false;
static uint8_t curr_mode;

void video_mode(uint8_t mode)
{
    if (mode > MAX_MODE)
        return;

    xvideo_mode(mode);
    is_mode_set = true;
    curr_mode = mode;

    switch (mode)
    {
    case BW_TEXT_MODE:
    {
        max_x = BW_TEXT_MAX_X;
        max_y = BW_TEXT_MAX_Y;
        break;
    }
    }
    return;
}

point_t *mk_point(uint16_t x, uint16_t y, uint8_t color)
{
    point_t *point = alloc(sizeof(point_t));
    if (!point)
        return NULL;

    point->x = x;
    point->y = y;
    point->color = color;

    return point;
}

boolean draw_point(point_t *point)
{
    boolean ret = false;
    if (!point || !is_mode_set)
        return false;

    if (point->x > max_x || point->y > max_y)
        return false;

    if (!is_mode_set)
        return false;

    switch (curr_mode)
    {
    case BW_TEXT_MODE:
    {
        ret = xdraw_point_bwt(point->x, point->y); // even though each argument is two byte, to maintain stack alignment
                                                   // , both will be pushed as 4 byte words
        break;
    }
    }

    return ret;
}