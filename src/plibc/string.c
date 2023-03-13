#include "string.h"

int memcmp(const void *a, const void *b, size_t n) {
    const char *x=a, *y=b;
    for (size_t i = 0; i < n; i++) {
        if (x[i] < y[i]) return -1;
        if (x[i] > y[i]) return 1;
    }
    return 0;
}
