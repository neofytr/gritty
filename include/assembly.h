#ifndef ASSEMBLY_H
#define ASSEMBLY_H

#include <stdint.h>

void xputchar(uint8_t chr);
void xwrite(uint8_t chr);
void xvideo_mode(uint8_t mode);
uint8_t xgetchar(void);
uint8_t xdraw_point_bwt(uint16_t x, uint16_t y);
uint8_t xdraw_point_bwg(uint16_t x, uint16_t y);
uint8_t xdraw_point_scg(uint16_t x, uint16_t y, uint8_t color);

#endif