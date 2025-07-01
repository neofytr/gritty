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

/**
 * draws a thick line between two points using bresenham's line algorithm.
 *
 * this implementation uses only integer arithmetic to plot a straight line
 * between two screen points, with support for configurable line thickness.
 * thickness is applied perpendicular to the main direction of the line.
 *
 * the algorithm determines whether to step in x or y (or both) at each point
 * based on how far the rasterized line has deviated from the ideal line.
 *
 * parameters:
 * - line: pointer to a line_t structure with endpoints, color, and thickness
 *
 * returns:
 * - true if the line was drawn successfully
 * - false if input is invalid or drawing failed
 */
boolean draw_line(line_t *line)
{
    // check if drawing mode is active and input is valid
    if (!line || !is_mode_set)
        return false;

    if (!line->one || !line->two)
        return false;

    if (line->thickness >= max_y)
        return false;

    // cache x and y values of both endpoints
    int16_t x0 = line->one->x;
    int16_t y0 = line->one->y;
    int16_t x1 = line->two->x;
    int16_t y1 = line->two->y;

    // compute absolute distances in x and y
    uint16_t dx = (x1 > x0) ? (x1 - x0) : (x0 - x1);
    uint16_t dy = (y1 > y0) ? (y1 - y0) : (y0 - y1);

    // determine step direction in x and y
    int16_t sx = (x0 < x1) ? 1 : -1;
    int16_t sy = (y0 < y1) ? 1 : -1;

    // initialize the error term
    int16_t err = dx - dy;

    // point to draw
    point_t point;
    point.color = line->color;

    // main loop: continue until we reach the end point
    while (x0 != x1 || y0 != y1)
    {
        // draw thickness around the core line pixel
        for (int16_t t = -(line->thickness / 2); t <= (line->thickness / 2); t++)
        {
            if (dx > dy)
            {
                // if line is more horizontal, draw thickness vertically
                point.x = x0;
                point.y = y0 + t;
            }
            else
            {
                // if line is more vertical, draw thickness horizontally
                point.x = x0 + t;
                point.y = y0;
            }

            // draw the point; return false if failed
            if (!draw_point(&point))
                return false;
        }

        // compute double the error for decision making
        int16_t err2 = 2 * err;

        // move in x if the error indicates we are below the ideal line
        if (err2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }

        // move in y if the error indicates we are off horizontally
        if (err2 < dx)
        {
            err += dx;
            y0 += sy;
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