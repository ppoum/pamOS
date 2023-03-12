#ifndef STDIO_H
#define STDIO_H

#include "../types.h"

#define EOF -1

void loadFont(uint32_t horizontalRes, uint32_t verticalRes);

int putchar(int c);
int puts(const char *str);

#endif