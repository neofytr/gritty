#ifndef ASSEMBLY_H
#define ASSEMBLY_H

#include <stdint.h>

// all these functions return negative of the error code upon error
// 0 upon success

void xputchar(uint8_t chr);
void xwrite(uint8_t chr);
void xvideo_mode(uint8_t mode);
uint8_t xgetchar(void);
uint8_t xdraw_point_bwt(uint16_t x, uint16_t y);
uint8_t xdraw_point_scg(uint16_t x, uint16_t y, uint8_t color);
uint16_t xget_more_err_info(uint16_t *err, uint16_t *act);
int16_t xopen_file(const char *filename, uint8_t mode);
int16_t xclose_file(int16_t file);
int16_t xread_file(int16_t file, uint16_t bytes, uint8_t *buf);
int16_t xwrite_file(int16_t file, uint16_t bytes, uint8_t *buf);
int16_t xseek_file(int16_t file, uint16_t offset, uint8_t whence);

#endif