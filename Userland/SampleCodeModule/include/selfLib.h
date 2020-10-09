#ifndef _SELF_LIB_H
#define _SELF_LIB_H

#define FOREGROUND 0 
#define BACKGROUND 1

#include <stdint.h>

char getChar();
uint8_t putChar(uint8_t c);
uint8_t printString(uint8_t * string);
uint8_t clear();
uint8_t nextScreen(uint8_t screen);
uint8_t printError(uint8_t * string);
uint8_t needScreen();
uint8_t initVisualEnvironment();
uint64_t addNewProcess(int (*function)(int, char **), int argc, char **argv,uint64_t execution);
uint64_t getPid();
uint64_t getSecondsElapsed();
uint64_t malloc2(unsigned nbytes);
void free2();


#endif