#ifndef IDT_H
#define IDT_H

#include "../types.h"

void loadIDT();
void setIDTEntry(int n, uintptr_t addr);

#endif
