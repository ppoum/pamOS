#include "keyboard.h"

#include "../types.h"
#include "../kernel/isr.h"

#include "../plibc/stdio.h"

void keyboardISR();

void initializeKeyboard() {
    registerIRQHandler(1, (irqh_t)keyboardISR);
}

void keyboardISR() {
    puts("Keyboard :)");
}