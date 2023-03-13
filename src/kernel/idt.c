#include "idt.h"

struct IDTRstruct {
    uint16_t limit;
    uintptr_t base;
} __attribute__((packed));

struct InterruptDescriptor {
    uint16_t offset1;  // Offset[15..0]
    uint16_t segSelect;  // Segment selector[15..0]
    uint8_t IST;  // IST[2..0], top 5 bits are reserved
    uint8_t attributes;  // P-DPL-0-GateType
    uint16_t offset2;  // Offset[31..16]
    uint32_t offset3;  // Offset[63..32]
    uint32_t reserved;
} __attribute__((packed));

struct InterruptDescriptor idt[256];


void loadIDT() {
    struct IDTRstruct idtr;
    idtr.base = (uintptr_t) &idt;
    // 16 bytes per entry, 256 entry, limit should be 1 less than actual size
    idtr.limit = 16 * 256 - 1;
    asm("lidt %0" : : "m" (idtr));
}

void setIDTEntry(int n, uintptr_t addr) {
    idt[n].offset1 = addr & 0xFFFF;
    idt[n].offset2 = (addr >> 16) & 0xFFFF;
    idt[n].offset3 = (addr >> 32) & 0xFFFFFFFF;
    idt[n].segSelect = 0x8;         // Points to 1st segment in GDT, called by 00 Privilege Level
    idt[n].attributes = (1 << 7)    // Present bit (must be one)
        | (0b11 << 5)               // DPL=0b11, all usermode apps have access to this interrupt
        | (0 << 4)                  // 0
        | (0b1110);                 // Interrupt gate
    idt[n].IST = 0;                 // Don't use IST
    idt[n].reserved = 0;            // Set reserved to 0 just in case
}