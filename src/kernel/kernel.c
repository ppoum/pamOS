#include "../types.h"

#include "../drivers/screen.h"

#include "../plib/stdio.h"

int main(uint64_t frameBufferBase, uint32_t horizontalRes, uint32_t verticalRes, uint32_t ppsl) {
    initializeScreen(frameBufferBase, horizontalRes, verticalRes, ppsl);
    
    // Paint background
    for (int row = 0; row < verticalRes; row++) {
        for (int col = 0; col < horizontalRes; col++) {
            uint8_t r = ((float)row / verticalRes) * 255;
            uint8_t b = ((float)col / horizontalRes) * 255;
            writePixel(row, col, r, 0, b);
        }
    }

    putchar('H');
    putchar('e');
    putchar('l');
    putchar('l');
    putchar('o');
    putchar(' ');
    putchar('W');
    putchar('o');
    putchar('r');
    putchar('l');
    putchar('d');
    putchar('!');
    putchar('\n');

    puts("Printing strings");

    for(;;);
}