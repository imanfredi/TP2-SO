#ifndef _STR_FUNCTIONS_KERNEL_H
#define _STR_FUNCTIONS_KERNEL_H

#include <stdint.h>

int strlen(const uint8_t *s);
uint8_t *strcpy(uint8_t dest[], const uint8_t source[]);
int strcmp(const uint8_t*s1,const uint8_t *s2);
int strncmp(const uint8_t *s1, const uint8_t *s2, int n);
void ftoa(double value, uint8_t *str, uint8_t precision);
void itoa(int value, uint8_t *str, int base);
uint32_t uintToBase(uint64_t value, uint8_t *buffer, uint32_t base);
int printf(const char *format, ...);
uint64_t stringHexaToNumber(uint8_t *string);
uint32_t uintToBaseWithLength(uint64_t value, uint8_t *buffer, uint32_t base, uint8_t size);

#endif