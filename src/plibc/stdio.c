#include "stdio.h"

#include "../drivers/screen.h"

extern char _binary_font_psf_start;

// PSF FONT LOGIC
#define PSF_FONT_MAGIC_BYTES 0x864AB572
typedef struct {
    uint32_t magic;
    uint32_t version;
    uint32_t headerSize;
    uint32_t flags;
    uint32_t numGlyph;
    uint32_t bytesPerGlyph;
    uint32_t height;
    uint32_t width;
} PSF_font;

PSF_font *font = (PSF_font *)&_binary_font_psf_start;
uint32_t CHAR_HOR, CHAR_VER;

// CURSOR
uint32_t CUR_X_POS, CUR_Y_POS;
uint32_t FG_COLOR = 0x00FFFFFF;  // White
uint32_t BG_COLOR = 0x00000000;  // Black

void loadFont(uint32_t horizontalRes, uint32_t verticalRes) {
    CUR_X_POS = 0;
    CUR_Y_POS = 0;

    CHAR_HOR = horizontalRes / font->width;
    CHAR_VER = verticalRes / font->height;
}

void setCursor(uint32_t x, uint32_t y) {
    CUR_X_POS = x;
    CUR_Y_POS = y;
}

uint32_t getCursorX() {
    return CUR_X_POS;
}

uint32_t getCursorY() {
    return CUR_Y_POS;
}

void setForegroundColor(uint32_t color) {
    FG_COLOR = color;
}

void setBackgroundColor(uint32_t color) {
    BG_COLOR = color;
}


int putchar(int c) {
    char character = (char) c;

    // Check for newline or CR
    if (character == '\n') {
        CUR_X_POS = 0;
        CUR_Y_POS++;  // TODO scroll
        return character;
    } else if (character == '\r') {
        CUR_X_POS = 0;
        return character;
    }

    uint8_t *glyph = (uint8_t *)font + font->headerSize + character * font->bytesPerGlyph;

    uint32_t topRow = CUR_Y_POS * font->height;
    uint32_t topCol = CUR_X_POS * font->width;

    for (int row = 0; row < font->height; row++) {
        for (int col = 0; col < font->width; col++) {
            if (*glyph & (1 << (7 - col))) {
                writePixel(topRow + row, topCol + col, FG_COLOR);
            } else if (BG_COLOR != COLOR_TRANSPARENT_BG)
                writePixel(topRow + row, topCol + col, BG_COLOR);
        }
        glyph++;
    }

    // Increment cursor
    CUR_X_POS++;
    if (CUR_X_POS >= CHAR_HOR) {
        CUR_X_POS = 0;
        CUR_Y_POS++;  // TODO Check if Y reached bottom of screen. If so,
                      // "scroll" down
    }
    return character;
}

int puts(const char *str) {
    while (str[0] != 0) {
        putchar(*str++);
    }
    return 1;
}

uint64_t fwrite(const void *s, uint64_t size, uint64_t nmemb) {
    for (int i = 0; i < size*nmemb; i++) {
        putchar(*(char*)s++);
    }
    return nmemb;
}