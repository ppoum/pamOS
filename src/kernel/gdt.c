#include "gdt.h"

#define GDT_SIZE 3

extern void reloadSegments();

struct SegmentDescriptor {
    uint16_t limitLow;
    uint16_t baseLow;
    uint8_t baseMid;
    // P-DPL-1-T-CE-RW-A -- P: present bit, DPL: Privilege level
    //   T: 1/0=code/data or stack, RW:read/write, A: accessed
    uint8_t accessByte;
    // Flags: G-DB-L-reserved  --  G: Granularity (is limit size in bytes or blocks of 4KiB), DB: 0/1=16-bit seg/32-bit seg, L: if 1, 64-bit, DB=0
    uint8_t flagsLimitHigh;  // top 4: flags, lower 4: limit high
    uint8_t baseHigh;
} __attribute__((packed));

struct GDTRstruct {
    uint16_t size;
    uintptr_t offset;
} __attribute__((packed));

struct SegmentDescriptor gdt[GDT_SIZE];

void loadGDT() {
    // First segment must always be null
    gdt[0].baseLow = 0;
    gdt[0].baseMid = 0;
    gdt[0].baseHigh = 0;
    gdt[0].limitLow = 0;
    gdt[0].flagsLimitHigh = 0;
    gdt[0].accessByte = 0;

    // Kernel code
    gdt[1].baseLow = 0;
    gdt[1].baseMid = 0;
    gdt[1].baseHigh = 0;
    gdt[1].limitLow = 0xFFFF;
    gdt[1].flagsLimitHigh = (0b1010 << 4) + 0xF;  // Flags define a 64-bit code segment
    // 1 - 00 (kernel segment) - 1 (data/code seg) - 1 (executable) - 0 (only executable by kernel) - 1 (readable) - 0 (access bit)
    gdt[1].accessByte = 0b10011010;

    // Kernel data
    gdt[2].baseLow = 0;
    gdt[2].baseMid = 0;
    gdt[2].baseHigh = 0;
    gdt[2].limitLow = 0xFFFF;
    gdt[2].flagsLimitHigh = (0b1100 << 4) + 0xF;  // Flags define 32-bit segment
    // 1 - 00 (kernel segment) - 1 (data/code seg) - 0 (not executable, data seg) - 0 (only exec by kernel) - 1 (writeable) - 0 (access bit)
    gdt[2].accessByte = 0b10010010;

    // Load struct into CPU
    struct GDTRstruct gdtr;
    gdtr.offset = (uintptr_t) &gdt;
    gdtr.size = sizeof(struct SegmentDescriptor) * GDT_SIZE - 1;
    asm("lgdt %0" : : "m"(gdtr));
    reloadSegments();
}