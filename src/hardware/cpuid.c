#include "cpuid.h"

extern void CPUID_get_vendor_asm(char* buf);

/// @brief Gets the CPU vendor tag
/// @param buf Pointer to 12 character long buffer
void CPUID_get_vendor(char* buf) {
    CPUID_get_vendor_asm(buf);
}

int CPUID_get_model() {
    uint32_t eax, unused;
    __cpuid(1, eax, unused, unused, unused);
    return eax & ~(0xF000F000);
}

int CPUID_get_max() {
    return __get_cpuid_max(0x80000000, NULL);
}

void CPUID_get_value(uint32_t level, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx) {
    uint32_t unused;
    if (eax == NULL) eax = &unused;
    if (ebx == NULL) ebx = &unused;
    if (ecx == NULL) ecx = &unused;
    if (edx == NULL) edx = &unused;
    __get_cpuid(level, eax, ebx, ecx, edx);
}

bool CPUID_check_ecx_feature(CPUID_feature_t feat) {
    uint32_t eax, unsued, ecx;
    __get_cpuid(1, &eax, &unsued, &ecx, &unsued);
    return ecx & feat ? true : false;
}

bool CPUID_check_edx_feature(CPUID_feature_t feat) {
    uint32_t eax, unsued, edx;
    __get_cpuid(1, &eax, &unsued, &unsued, &edx);
    return edx & feat ? true : false;
}