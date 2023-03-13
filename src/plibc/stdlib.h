#ifndef STDLIB_H
#define STDLIB_H

// 64-bit number can take up 65 bytes in binary (64+1 for \0), lower in larger bases
#define ITOA_MAX_LEN 65

char *itoa(int val, char *str, int base);
int atoi(const char *s);

#endif