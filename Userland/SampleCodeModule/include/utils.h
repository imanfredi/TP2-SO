#ifndef _UTILS_
#define _UTILS_
#include <stdint.h>
#define HEXADIRSIZE 16
#define EXIT_CODE 251
#define CLEAR_SCREEN_CODE 252

int isOperator(char  character);
int isDigit(char c);
int toNumber(char character);
uint64_t pow(uint64_t base, uint64_t exp);
int isDigitOrPoint(char c);
void mathPow(int base, int exp, double *number);
void Abs(double number, double *aux);
int isHexaDir(char * buffer);
int isHexaLetter(char c);
void * memset(void * destination, int32_t character, uint64_t length);
#endif