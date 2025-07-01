#include <shapes.h>
#include <assembly.h>
#include <gritty.h>

static uint16_t max_x, max_y;
static boolean is_mode_set = false;
static uint8_t curr_mode;

static boolean draw_filled_rect(int x, int y, int w, int h, uint8_t color);
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
        max_x = BW_TEXT_MAX_X;
        max_y = BW_TEXT_MAX_Y;
        break;
    }
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
    if (!point || !is_mode_set)
        return false;

    if (point->x > max_x || point->y > max_y)
        return false;

    switch (curr_mode)
    {
    case BW_TEXT_MODE:
        return xdraw_point_bwt(point->x, point->y);
    }

    return false;
}

static boolean draw_filled_rect(int x, int y, int w, int h, uint8_t color)
{
    point_t point;
    point.color = color;

    for (int j = 0; j < h; ++j)
    {
        for (int i = 0; i < w; ++i)
        {
            point.x = x + i;
            point.y = y + j;

            if (point.x > max_x || point.y > max_y)
                continue;

            if (!draw_point(&point))
                return false;
        }
    }
    return true;
}

line_t *mk_line(point_t *one, point_t *two, uint16_t thickness, uint8_t color)
{
    if (!one || !two || !thickness)
        return NULL;

    line_t *line = alloc(sizeof(line_t));
    if (!line)
        return NULL;

    line->one = one;
    line->two = two;
    line->thickness = thickness;
    line->color = color;

    return line;
}

rectangle_t *mk_rect(point_t *one, point_t *three, uint16_t thickness, uint8_t fg_color, uint8_t bg_color, boolean filled)
{
    if (!one || !three || !thickness)
        return NULL;

    rectangle_t *rect = alloc(sizeof(rectangle_t));
    if (!rect)
        return NULL;

    rect->one = one;
    rect->three = three;
    rect->thickness = thickness;
    rect->fg_color = fg_color;
    rect->bg_color = bg_color;
    rect->filled = filled;

    return rect;
}

boolean draw_rect(rectangle_t *rect)
{
    if (!rect || !rect->one || !rect->three)
        return false;

    int x0 = rect->one->x;
    int y0 = rect->one->y;
    int x1 = rect->three->x;
    int y1 = rect->three->y;

    // ensure x0 < x1 and y0 < y1
    if (x0 > x1)
    {
        int tmp = x0;
        x0 = x1;
        x1 = tmp;
    }
    if (y0 > y1)
    {
        int tmp = y0;
        y0 = y1;
        y1 = tmp;
    }

    int thickness = rect->thickness;
    uint8_t border = rect->bg_color;
    uint8_t fill = rect->fg_color;

    // top border
    if (!draw_filled_rect(x0, y0, x1 - x0 + 1, thickness, border))
        return false;

    // bottom border
    if (!draw_filled_rect(x0, y1 - thickness + 1, x1 - x0 + 1, thickness, border))
        return false;

    // left border
    if (!draw_filled_rect(x0, y0 + thickness, thickness, y1 - y0 + 1 - 2 * thickness, border))
        return false;

    // right border
    if (!draw_filled_rect(x1 - thickness + 1, y0 + thickness, thickness, y1 - y0 + 1 - 2 * thickness, border))
        return false;

    // fill inside if requested
    if (rect->filled)
    {
        if (!draw_filled_rect(x0 + thickness, y0 + thickness,
                              x1 - x0 + 1 - 2 * thickness,
                              y1 - y0 + 1 - 2 * thickness,
                              fill))
            return false;
    }

    return true;
}
