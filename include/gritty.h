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

#define INPUT ((fileHandle_t)0)
#define OUTPUT ((fileHandle_t)1)
#define ERROR ((fileHandle_t)2)

#define INHERITABLE ((uint8_t)0x00)
#define PRIVATE ((uint8_t)0x20)

extern uint8_t __heap; // symbol exported by the linker script

#define dealloc(x) ;

int16_t print(const char *str);
void *alloc(uint16_t size);
void freeall(void);
fileHandle_t openFile(const char *filename, uint8_t accessMode, uint8_t sharingMode, uint8_t inheritenceMode); // returns the error code in errnum and -1 on error
boolean closeFile(fileHandle_t fileHandle);                                                                    // returns the error code in errnum and -1 on error

// returns the number of bytes read on success, -1 on error
// if the returned value is < bytes, then a partial read occurred due to EOF
// if the returned value is 0, no data was read, and EOF occurred before read
int16_t readFile(fileHandle_t fileHandle, uint16_t bytes, uint8_t *buffer);

// returns the number of bytes written on success, -1 on error
// if the returned value is < bytes, then a partial write occurred
// this function can be used to truncate a file to the current file position by writing zero bytes
int16_t writeFile(fileHandle_t fileHandle, uint16_t bytes, uint8_t *buffer);

// returns the number of bytes written to the buffer (except the null-byte) on success, -1 on error
// this function can be used to print a formatted string to a buffer
// this function is strict about the last null byte; it will always ensure that there is a null-byte at the end of the buffer
// if max_len is 1, buf[0] will be the null-byte
int16_t printFormattedToBuffer(uint8_t *buf, uint16_t max_len, const char *format, ...);

// prints a formatted string to stdout
// returns the number of bytes written to the STDOUT, except the null-byte, on success, and -1 on error
int16_t printFormatted(const char *format, ...);

// prints a formatted string to the specified file
// returns the number of bytes written to the file, except the null-byte, on success, and -1 on error
int16_t printFormattedToFile(fileHandle_t fileHandle, const char *format, ...);

#endif