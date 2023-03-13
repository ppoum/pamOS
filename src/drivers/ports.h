#ifndef PORTS_H
#define PORTS_H

#include "../types.h"

#define PORT_RTC_INDEX 0x70
#define PORT_RTC_DATA 0x71

uint8_t portByteIn(uint16_t port);
void portByteOut(uint16_t port, uint8_t data);

uint16_t portWordIn(uint16_t port);
void portWordOut(uint16_t port, uint16_t data);

#endif