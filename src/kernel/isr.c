#include "isr.h"

#include "../plibc/stdio.h"
#include "idt.h"

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();

const char *intelErrorMsg[] = {"Division by 0",
                               "NMI Interrupt",
                               "Breakpoint",
                               "Overflow",
                               "BOUND Range Exceeded",
                               "Invalid Opcode",
                               "Device Not Available (no Math co-CPU)",
                               "Double Fault",
                               "CoProcessor Segment Overrun",
                               "Invalid TSS",
                               "Segment Not Present",
                               "Stack Segment Fault",
                               "General Protection",
                               "Page Fault",
                               "Reserved",
                               "Floating-Point Error",
                               "Alignment Check",
                               "Machine Check",
                               "SIMD Floating-Point Exception",
                               "Virtualization Exception",
                               "Control Protection Exception"};

irqh_t irqHandlers[256];

void setupHandlers() {
    setIDTEntry(0, (uintptr_t)isr0);
    setIDTEntry(1, (uintptr_t)isr1);
    setIDTEntry(2, (uintptr_t)isr2);
    setIDTEntry(3, (uintptr_t)isr3);
    setIDTEntry(4, (uintptr_t)isr4);
    setIDTEntry(5, (uintptr_t)isr5);
    setIDTEntry(6, (uintptr_t)isr6);
    setIDTEntry(7, (uintptr_t)isr7);
    setIDTEntry(8, (uintptr_t)isr8);
    setIDTEntry(9, (uintptr_t)isr9);
    setIDTEntry(10, (uintptr_t)isr10);
    setIDTEntry(11, (uintptr_t)isr11);
    setIDTEntry(12, (uintptr_t)isr12);
    setIDTEntry(13, (uintptr_t)isr13);
    setIDTEntry(14, (uintptr_t)isr14);
    setIDTEntry(15, (uintptr_t)isr15);
    setIDTEntry(16, (uintptr_t)isr16);
    setIDTEntry(17, (uintptr_t)isr17);
    setIDTEntry(18, (uintptr_t)isr18);
    setIDTEntry(19, (uintptr_t)isr19);
    setIDTEntry(20, (uintptr_t)isr20);
    setIDTEntry(21, (uintptr_t)isr21);

    // Setup PIC


    loadIDT();
}

// Private functions (used by isr_asm.asm)
void intelInterruptHandler(int isrCode) {
    setCursor(0, 0);
    setForegroundColor(COLOR_RED);
    setBackgroundColor(COLOR_WHITE);
    puts("EXCEPTION!!\n");
    puts(intelErrorMsg[isrCode]);

    // Halt CPU
    asm("cli; hlt" ::);
}

void genericInterruptHandler(int isrCode) {}