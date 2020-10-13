#ifndef _STR_FUNCTIONS_H
#define _STR_FUNCTIONS_H

#include <stdint.h>
#define BULLET_POINT 254
#define TRUE 1
#define FALSE 0

int strlen(const uint8_t *s);
uint8_t *strcpy(uint8_t dest[], const uint8_t source[]);
int strcmp(const uint8_t *s1, const uint8_t *s2);
int strncmp(const uint8_t *s1, const uint8_t *s2, int n);
void ftoa(double value, uint8_t *str, uint8_t precision);
void itoa(int value, uint8_t *str, int base);
uint32_t uintToBase(uint64_t value, uint8_t *buffer, uint32_t base);
int printf(const char *format, ...);
uint64_t stringHexaToNumber(uint8_t *string);
uint32_t uintToBaseWithLength(uint64_t value, uint8_t *buffer, uint32_t base, uint8_t size);
void *memcpy(void *destination, const void *source, uint64_t length);
int atoi(const uint8_t *arr);
int strtok(char token, char **dest, char *source, int max);
uint8_t * strcat(uint8_t * dest,uint8_t * src);

#endif