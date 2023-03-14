#include "apic.h"

#include "./cpuid.h"
#include "./msr.h"

// Useful documentation
// https://www.naic.edu/~phil/software/intel/318148.pdf  - section 2.2 (p. 16)

#define IA32_APIC_BASE_MSR      0x1B
#define APIC_MSR_DISABLE_BSP    ~(1 << 8)
#define APIC_MSR_EXTD           (1 << 10)
#define APIC_MSR_EN             (1 << 11)

uintptr_t apic_base;

bool apic_check_present() {
    return CPUID_check_ecx_feature(CPUID_FEAT_ECX_X2APIC);
}

void apic_enable() {
    uint64_t apic_msr_val = msr_read(IA32_APIC_BASE_MSR);
    apic_msr_val |= APIC_MSR_EN | APIC_MSR_EXTD;    // Enable in extended mode (x2APIC)
    apic_msr_val &= APIC_MSR_DISABLE_BSP;           // Not running on BSP
    msr_write(IA32_APIC_BASE_MSR, apic_msr_val);
}