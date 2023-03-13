#ifndef SCREEN_H
#define SCREEN_H

#include "../types.h"

void initializeScreen(uint64_t frameBufferBase, uint32_t horizontalRes, uint32_t verticalRes, uint32_t ppsl);

uint32_t getScreenHorizontalRes();
uint32_t getScreenVerticalRes();

void writePixel(uint32_t row, uint32_t col, uint32_t color);

#endif