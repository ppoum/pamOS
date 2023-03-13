#ifndef ISR_H
#define ISR_H

typedef void (*irqh_t)();

void setupHandlers();
void registerIRQHandler(int n, irqh_t handlerAddr);

#endif
