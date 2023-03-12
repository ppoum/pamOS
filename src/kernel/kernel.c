#include "../types.h"

int main(uint64_t frameBufferBase, uint32_t hor, uint32_t ver, uint32_t PPSL) {
    uint32_t* videoBuf = (uint32_t*)frameBufferBase;
    
    for (int row = 0; row < ver; row++) {
        for (int col = 0; col < hor; col++) {
            uint8_t r = ((float)row / ver) * 255;
            uint8_t b = ((float)col / hor) * 255;
            videoBuf[row*PPSL + col] = (r << 16) + b;
        }
    }

    for(;;);
}