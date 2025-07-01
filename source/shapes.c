#include <shapes.h>
#include <assembly.h>

void video_mode(uint8_t mode)
{
    if (mode > MAX_MODE)
        return;

    xvideo_mode(mode);
    return;
}