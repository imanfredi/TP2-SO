#ifndef _STR_FUNCTIONS_KERNEL_H
#define _STR_FUNCTIONS_KERNEL_H

#include <stdint.h>

int strlen(const uint8_t *s);
uint32_t uintToBase(uint64_t value, uint8_t *buffer, uint32_t base);
uint32_t uintToBaseWithLength(uint64_t value, uint8_t *buffer, uint32_t base, uint8_t size);
int strcpy(char dest[], const char source[]);
#endif