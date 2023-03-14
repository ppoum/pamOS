#include "../types.h"

#include "../drivers/screen.h"
#include "../drivers/keyboard.h"

#include "../hardware/cpuid.h"
#include "../hardware/apic.h"
#include "../hardware/msr.h"

#include "./gdt.h"
#include "./isr.h"

#include "../plibc/stdio.h"
#include "../plibc/stdlib.h"
#include "../plibc/string.h"

int main(uint64_t frameBufferBase, uint32_t horizontalRes, uint32_t verticalRes, uint32_t ppsl) {
    // Setup (mask interrupts)
    asm("cli");
    loadGDT();
    setupHandlers();
    initializeScreen(frameBufferBase, horizontalRes, verticalRes, ppsl);
    initializeKeyboard();
    asm("sti");
    
    // Paint background
    for (int row = 0; row < verticalRes; row++) {
        for (int col = 0; col < horizontalRes; col++) {
            uint8_t r = ((float)row / verticalRes) * 255;
            uint8_t b = ((float)col / horizontalRes) * 255;
            uint32_t color = (r << 16) + b;
            writePixel(row, col, color);
        }
    }

    char vendorBuf[12];
    CPUID_get_vendor(vendorBuf);

    if (memcmp(vendorBuf, CPUID_VENDOR_AMD, 12) == 0 || memcmp(vendorBuf, CPUID_VENDOR_AMD_OLD, 12) == 0) {
        setForegroundColor(COLOR_RED);
        puts("CPU vendor: AMD\n");
    } else if (memcmp(vendorBuf, CPUID_VENDOR_INTEL, 12) == 0) {
        setForegroundColor(COLOR_BLUE);
        puts("CPU vendor: Intel\n");
    } else {
        setForegroundColor(COLOR_WHITE);
        setBackgroundColor(COLOR_RED);
        puts("CPU vendor: Unknown\n");
    }

    char cpu_model[ITOA_MAX_LEN];
    itoa(CPUID_get_model(), cpu_model, 10);
    puts("CPU model: ");
    puts(cpu_model);
    putchar('\n');

    // Check for some features
    if (apic_check_present()) {
        setForegroundColor(COLOR_GREEN);
        setBackgroundColor(COLOR_TRANSPARENT_BG);
        puts("APIC present\n");
    } else {
        setForegroundColor(COLOR_RED);
        setBackgroundColor(COLOR_WHITE);
        puts("No APIC\n");
    }

    if (msr_check_present()) {
        setForegroundColor(COLOR_GREEN);
        setBackgroundColor(COLOR_TRANSPARENT_BG);
        puts("MSR present\n");
    } else {
        setForegroundColor(COLOR_RED);
        setBackgroundColor(COLOR_WHITE);
        puts("No MSR\n");
    }

    // uint32_t misc;
    // CPUID_get_value(0x01, NULL, NULL, &misc, NULL);
    // char buf[ITOA_MAX_LEN];
    // itoa(misc & 0b1000, buf, 10);
    // puts(buf);

    uint32_t test;
    // CPUID_get_value(0x16, NULL, NULL, &test, NULL);
    CPUID_get_value(0x0, &test, NULL, NULL, NULL);

    // uint32_t bus_freq;
    // CPUID_get_value(0x16, &bus_freq, NULL, NULL, NULL);
    // bus_freq &= 0xFFFF;
    // char buf[ITOA_MAX_LEN];
    // itoa(bus_freq, buf, 10);
    // // char buf[ITOA_MAX_LEN];
    // // int max = CPUID_get_max();
    // // itoa(max, buf, 10);
    // puts("Bus frequency: ");
    // puts(buf);
    // putchar('\n');

    for(;;);
}
