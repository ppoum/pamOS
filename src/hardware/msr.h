#ifndef MSR_H
#define MSR_H

#include "../types.h"

bool msr_check_present();

uint64_t msr_read(uint32_t msr);
void msr_write(uint32_t msr, uint64_t data);

#endif
