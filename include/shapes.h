#include <base.h>

#define BW_TEXT_MODE 0
#define BW_TEXT_MAX_X 40
#define BW_TEXT_MAX_Y 25

#define MAX_MODE 13

typedef struct packed
{
    uint16_t x, y;
    uint8_t color : 4; // our color pallette supports c0 to cfff (16 colors), so we need only 4 bits
} point_t;

void video_mode(uint8_t mode);
