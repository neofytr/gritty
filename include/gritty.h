#ifndef GRITTY_H
#define GRITTY_H

#include <stdint.h>

#define true 1
#define false 0

#define NULL ((void *)0)

typedef uint8_t bool;

extern uint8_t __heap; // symbol exported by the linker script

void print(uint8_t *str);
void putchar(uint8_t chr);
void *alloc(uint16_t size);
void freeall(void);

#endif