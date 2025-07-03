#ifndef GRITTY_H
#define GRITTY_H

#include <stdint.h>
#include <base.h>
#include <shapes.h>

typedef int16_t fileHandle_t;

#define READ_ONLY 0x0000
#define WRITE_ONLY 0x0001
#define READ_WRITE 0x0002

#define EXCLUSIVE_ACCESS 0x0000
#define DENY_OTHERS_READWRITE 0x0001
#define DENY_OTHERS_WRITE 0x0010
#define DENY_OTHERS_WRITE 0x0011
#define EVERYONE_FULL_ACCESS 0x0100

#define PRIVATE 0x0001
#define INHERITABLE 0x0000

extern uint8_t __heap; // symbol exported by the linker script

void print(uint8_t *str);
void putchar(uint8_t chr);
uint8_t getchar(void);
void *alloc(uint16_t size);
void freeall(void);
fileHandle_t openFile(const char *filename, uint8_t accessMode, uint8_t sharingMode, uint8_t inheritenceMode); // returns the error code in errnum and -1 on error
int16_t closeFile(fileHandle_t fileHandle);                                                                    // returns the error code in errnum and -1 on error

#endif