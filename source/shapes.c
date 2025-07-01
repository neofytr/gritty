#include <shapes.h>
#include <assembly.h>

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

boolean draw_point(point_t *point)
{
    if (!point || !is_mode_set)
        return false;

    if (point->x > max_x || point->y > max_y)
        return false;

    if (!is_mode_set)
        return false;
}