#include "msr.h"

#include "cpuid.h"

bool msr_check_present() { return CPUID_check_edx_feature(CPUID_FEAT_EDX_MSR); }

uint64_t msr_read(uint32_t msr) {
    uint32_t lo, hi;
    asm("rdmsr" : "=d"(hi), "=a"(lo) : "c"(msr));
    return ((uint64_t)hi << 32) + lo;
}

void msr_write(uint32_t msr, uint64_t data) {
    uint32_t lo = data & 0xFFFFFFFF;
    uint32_t hi = (data >> 32);
    asm("wrmsr" :: "d"(hi), "a"(lo), "c"(msr));
}