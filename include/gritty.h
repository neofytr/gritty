#ifndef GRITTY_H
#define GRITTY_H

#include <stdint.h>

#define true 1
#define false 0

typedef uint8_t bool;

void print(uint8_t *str);
void putchar(uint8_t chr);

#endif