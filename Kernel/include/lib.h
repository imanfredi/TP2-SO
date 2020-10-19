#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void *memset(void *destination, int32_t character, uint64_t length);
void *memcpy(void *destination, const void *source, uint64_t length);

uint8_t *cpuVendor(uint8_t *result);
uint64_t cpuModel();

int _temperature(void);
void *getInitialSP();
void *callTimerTick();
int _xchg(int * lock,int value);
#endif