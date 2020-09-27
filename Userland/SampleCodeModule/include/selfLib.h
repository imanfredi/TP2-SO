#ifndef _SELF_LIB_H
#define _SELF_LIB_H
#include <stdint.h>

char getChar();
uint8_t putChar(uint8_t c);
uint8_t printString(uint8_t * string);
uint8_t clear();
uint8_t nextScreen(uint8_t screen);
uint8_t printError(uint8_t * string);
uint8_t needScreen();
uint8_t initVisualEnvironment();
void addNewProcess(int (*function)(int, char **), int argc, char **argv);
void exit();
uint64_t getPid();
uint64_t getSecondsElapsed();


#endif