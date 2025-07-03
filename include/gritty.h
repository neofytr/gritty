#ifndef GRITTY_H
#define GRITTY_H

#include <stdint.h>
#include <base.h>
#include <shapes.h>

typedef int16_t fileHandle_t;

#include <stdint.h>

#define READ_ONLY ((uint8_t)0x00)
#define WRITE_ONLY ((uint8_t)0x01)
#define READ_WRITE ((uint8_t)0x02)

#define EXCLUSIVE_ACCESS ((uint8_t)0x00) // deny all
#define DENY_OTHERS_READWRITE ((uint8_t)0x04)
#define DENY_OTHERS_WRITE ((uint8_t)0x08)
#define DENY_OTHERS_READ ((uint8_t)0x0C)
#define EVERYONE_FULL_ACCESS ((uint8_t)0x10)

// Inheritance flag (bit 4)
#define INHERITABLE ((uint8_t)0x00)
#define PRIVATE ((uint8_t)0x20)

extern uint8_t __heap; // symbol exported by the linker script

void print(uint8_t *str);
void putchar(uint8_t chr);
uint8_t getchar(void);
void *alloc(uint16_t size);
void freeall(void);
fileHandle_t openFile(const char *filename, uint8_t accessMode, uint8_t sharingMode, uint8_t inheritenceMode); // returns the error code in errnum and -1 on error
boolean closeFile(fileHandle_t fileHandle);                                                                    // returns the error code in errnum and -1 on error

// returns the number of bytes read on success, -1 on error
// if the returned value is < bytes, then a partial read occurred due to EOF
// if the returned value is 0, no data was read, and EOF occurred before read
int16_t readFile(fileHandle_t fileHandle, uint16_t bytes, uint8_t *buffer);

int16_t writeFile(fileHandle_t fileHandle, uint16_t bytes, uint8_t *buffer);
#endif