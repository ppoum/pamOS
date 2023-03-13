#include "ports.h"

uint8_t portByteIn(uint16_t port) {
    uint8_t portOut;
    asm("in %%dx, %%al" : "=a" (portOut) : "d" (port));
    return portOut;
}

void portByteOut(uint16_t port, uint8_t data) {
    asm("out %%al, %%dx" : : "a" (data), "d" (port));
}

uint16_t portWordIn(uint16_t port) {
    uint16_t portOut;
    asm("in %%dx, %%ax" : "=a" (portOut) : "d" (port));
    return portOut;
}

void portWordOut(uint16_t port, uint16_t data) {
    asm("out %%ax, %%dx" : : "a" (data), "d" (port));
}