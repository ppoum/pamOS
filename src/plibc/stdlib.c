#include "stdlib.h"

#include "../types.h"

char *itoa(int val, char *str, int base) {
    static const char *digits = "0123456789ABCDEF";

    // TODO Panic if base != 10
    // TODO Implement other bases (up to 16?)

    if (val == 0) {
        str[0] = '0';
        str[1] = '\0';
        return str;
    }

    char stack[ITOA_MAX_LEN];
    size_t stackSize = 0;

    while (val != 0) {
        stack[stackSize++] = digits[val % base];
        val /= base;
    }

    // Pop stack to reverse order
    size_t i = 0;
    while (stackSize > 0) {
        str[i++] = stack[--stackSize];
    }
    str[i] = '\0';

    return str;
}

int atoi(const char *s) {
    int i = 0;

    while (*s != 0) {
        if (*s < '0' || *s > '9') {
            // TODO Panic
        }
        i += *s - '0';  // Convert from ascii to binary
        i *= 10;  // Shift left
        s++;
    }

    return i;
}