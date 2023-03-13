#ifndef STDIO_H
#define STDIO_H

#include "../types.h"

#define EOF -1

#define COLOR_RED   0x00FF0000
#define COLOR_GREEN 0x0000FF00
#define COLOR_BLUE  0x000000FF
#define COLOR_WHITE 0x00FFFFFF
#define COLOR_BLACK 0x00000000
#define COLOR_PINK  0x00FF00FF

#define COLOR_TRANSPARENT_BG 0xDEADBEEF

void loadFont(uint32_t horizontalRes, uint32_t verticalRes);

void setCursor(uint32_t x, uint32_t y);
uint32_t getCursorX();
uint32_t getCursorY();

void setForegroundColor(uint32_t color);
void setBackgroundColor(uint32_t color);


int putchar(int c);
int puts(const char *str);
uint64_t fwrite(const void *s, size_t size, uint64_t nmemb);

#endif