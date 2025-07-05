#include <shapes.h>
#include <assembly.h>
#include <errnum.h>
#include <gritty.h>

static uint16_t max_x, max_y;
static boolean is_mode_set = false;
static uint8_t curr_mode;

static boolean draw_filled_rect(int x, int y, int w, int h, uint8_t color);

void video_mode(uint8_t mode)
{
    if (mode > MAX_MODE)
    {
        errnum = ERR_INVALID_ARGS;
        if (RETURN_ACTION)
            action = ACTION_FIX_ARGS;
        return;
    }

    switch (mode)
    {
    case BWT_MODE:
        max_x = BWT_MAX_X;
        max_y = BWT_MAX_Y;
        break;
    case SCG_MODE:
        max_x = SCG_MAX_X;
        max_y = SCG_MAX_Y;
        break;
    default:
        errnum = ERR_INVALID_ARGS;
        if (RETURN_ACTION)
            action = ACTION_FIX_ARGS;
        return;
    }

    xvideo_mode(mode);
    is_mode_set = true;
    curr_mode = mode;

    errnum = ERR_NO_ERR;
    if (RETURN_ACTION)
        action = ACTION_NO_ACTION;
    return;
}

point_t *mk_point(uint16_t x, uint16_t y, uint8_t color)
{
    point_t *point = alloc(sizeof(point_t));
    if (!point)
    {
        return NULL;
    }

    point->x = x;
    point->y = y;
    point->color = color;

    errnum = ERR_NO_ERR;
    if (RETURN_ACTION)
        action = ACTION_NO_ACTION;
    return point;
}

boolean draw_point(point_t *point)
{
    if (!point || !is_mode_set)
    {
        errnum = ERR_INVALID_ARGS;
        if (RETURN_ACTION)
            action = ACTION_FIX_ARGS;
        return false;
    }

    if (!is_mode_set)
    {
        errnum = ERR_VIDEOMODE_NOT_SET;
        if (RETURN_ACTION)
            action = ACTION_SET_VIDEOMODE;
        return false;
    }

    if (point->x > max_x || point->y > max_y)
    {
        errnum = ERR_SCREEN_BOUNDS;
        if (RETURN_ACTION)
            action = ACTION_FIX_DRAWING_BOUNDS;
        return false;
    }

    errnum = ERR_NO_ERR;
    if (RETURN_ACTION)
        action = ACTION_NO_ACTION;

    switch (curr_mode)
    {
    case BWT_MODE:
        return xdraw_point_bwt(point->x, point->y);
    case SCG_MODE:
        if (point->color >= 16)
        {
            errnum = ERR_INVALID_COLOR;
            if (RETURN_ACTION)
                action = ACTION_FIX_COLOR;
            return false;
        }
        return xdraw_point_scg(point->x, point->y, point->color);
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

            // skip pixels outside screen bounds
            if (point.x > max_x || point.y > max_y)
                continue;

            if (!draw_point(&point))
                return false; // draw_point would set the errnum/action upon return
        }
    }
    return true;
}

line_t *mk_line(point_t *one, point_t *two, uint16_t thickness, uint8_t color)
{
    if (!one || !two || !thickness)
    {
        errnum = ERR_INVALID_ARGS;
        if (RETURN_ACTION)
            action = ACTION_FIX_ARGS;
        return NULL;
    }

    line_t *line = alloc(sizeof(line_t));
    if (!line)
        return NULL;

    line->one = one;
    line->two = two;
    line->thickness = thickness;
    line->color = color;

    errnum = ERR_NO_ERR;
    if (RETURN_ACTION)
        action = ACTION_NO_ACTION;
    return line;
}

rectangle_t *mk_rect(point_t *one, point_t *three, uint16_t thickness, uint8_t fg_color, uint8_t bg_color, boolean filled)
{
    if (!one || !three || !thickness)
    {
        errnum = ERR_INVALID_ARGS;
        if (RETURN_ACTION)
            action = ACTION_FIX_ARGS;
        return NULL;
    }

    rectangle_t *rect = alloc(sizeof(rectangle_t));
    if (!rect)
        return NULL;

    rect->one = one;
    rect->three = three;
    rect->thickness = thickness;
    rect->fg_color = fg_color;
    rect->bg_color = bg_color;
    rect->filled = filled;

    errnum = ERR_NO_ERR;
    if (RETURN_ACTION)
        action = ACTION_NO_ACTION;
    return rect;
}

boolean draw_rect(rectangle_t *rect)
{
    if (!rect || !rect->one || !rect->three)
    {
        errnum = ERR_INVALID_ARGS;
        if (RETURN_ACTION)
            action = ACTION_FIX_ARGS;
        return false;
    }

    if (!is_mode_set)
    {
        errnum = ERR_VIDEOMODE_NOT_SET;
        if (RETURN_ACTION)
            action = ACTION_SET_VIDEOMODE;
        return false;
    }

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

    // check if rectangle is entirely outside screen bounds
    if (x0 > max_x || y0 > max_y || x1 < 0 || y1 < 0)
    {
        errnum = ERR_SCREEN_BOUNDS;
        if (RETURN_ACTION)
            action = ACTION_FIX_DRAWING_BOUNDS;
        return false;
    }

    int thickness = rect->thickness;
    uint8_t border = rect->bg_color;
    uint8_t fill = rect->fg_color;

    // validate colors for scg mode
    if (curr_mode == SCG_MODE)
    {
        if (border >= 16 || fill >= 16)
        {
            errnum = ERR_INVALID_COLOR;
            if (RETURN_ACTION)
                action = ACTION_FIX_COLOR;
            return false;
        }
    }

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

    errnum = ERR_NO_ERR;
    if (RETURN_ACTION)
        action = ACTION_NO_ACTION;
    return true;
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
    {
        errnum = ERR_INVALID_ARGS;
        if (RETURN_ACTION)
            action = ACTION_FIX_ARGS;
        return false;
    }

    if (!line->one || !line->two)
    {
        errnum = ERR_INVALID_ARGS;
        if (RETURN_ACTION)
            action = ACTION_FIX_ARGS;
        return false;
    }

    if (line->thickness >= max_y)
    {
        errnum = ERR_INVALID_ARGS;
        if (RETURN_ACTION)
            action = ACTION_FIX_ARGS;
        return false;
    }

    if (!is_mode_set)
    {
        errnum = ERR_VIDEOMODE_NOT_SET;
        if (RETURN_ACTION)
            action = ACTION_SET_VIDEOMODE;
        return false;
    }

    // validate color for scg mode
    if (curr_mode == SCG_MODE && line->color >= 16)
    {
        errnum = ERR_INVALID_COLOR;
        if (RETURN_ACTION)
            action = ACTION_FIX_COLOR;
        return false;
    }

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

    errnum = ERR_NO_ERR;
    if (RETURN_ACTION)
        action = ACTION_NO_ACTION;
    return true;
}