#ifndef GRITTY_H
#define GRITTY_H

#include <stdint.h>

extern uint8_t __heap; // symbol exported by the linker script

void print(uint8_t *str);
void putchar(uint8_t chr);
void *alloc(uint16_t size);
void freeall(void);

#endif