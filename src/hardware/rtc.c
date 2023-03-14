#include "rtc.h"

#include "../drivers/ports.h"

// CMOS memory map http://www.bioscentral.com/misc/cmosmap.htm

void initializeRTC() {
    portByteOut(PORT_RTC_INDEX, 0x8B);  // Select register B, also disable NMI
    char currVal = portByteIn(PORT_RTC_DATA);
    portByteOut(PORT_RTC_INDEX, 0x8B);  // set index to same val
    portByteOut(PORT_RTC_DATA, currVal | 0x40);  // Enable IRQ with rate of 1024Hz
}