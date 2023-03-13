#include "screen.h"
#include "../plibc/stdio.h"

// GLOBAL VARIABLES
uint32_t* FRAME_BUFFER;
uint32_t VERTICAL_RES, HORIZONTAL_RES, PPSL;


void initializeScreen(uint64_t frameBufferBase, uint32_t horizontalRes, uint32_t verticalRes, uint32_t ppsl) {
    FRAME_BUFFER = (uint32_t*) frameBufferBase;
    VERTICAL_RES = verticalRes;
    HORIZONTAL_RES = horizontalRes;
    PPSL = ppsl;

    loadFont(horizontalRes, verticalRes);
}

uint32_t getScreenHorizontalRes() {
    return HORIZONTAL_RES;
}

uint32_t getScreenVerticalRes() {
    return VERTICAL_RES;
}

void writePixel(uint32_t row, uint32_t col, uint32_t color) {
    FRAME_BUFFER[row * PPSL + col] = color;
}
