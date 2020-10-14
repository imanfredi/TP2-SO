#ifndef _SELF_LIB_H
#define _SELF_LIB_H

#define FOREGROUND 0
#define NULL 0
#define STDIN 0
#define STDOUT 1
#define BACKGROUND 1


#include <stdint.h>

int getChar();
uint8_t putChar(uint8_t c);
uint8_t printString(uint8_t *string);
uint8_t clear();
uint8_t nextScreen(uint8_t screen);
uint8_t printError(uint8_t *string);
uint8_t needScreen();
uint8_t initVisualEnvironment();
uint64_t addNewProcess(int (*function)(int, char **), int argc, char **argv, uint64_t execution, int fd[]);
uint64_t getPid();
uint64_t getSecondsElapsed();
uint64_t malloc2(unsigned nbytes);
void free2();
void * sem_open(char * name,int value);
int sem_close(void * sem);
int sem_wait(void * sem);
int sem_post(void * sem);
void memInfo();
void semInfo();
int pipeOpen(char* name);
int closePipe(int fd);
int changeValue(void * sem,int value);
int writeInPipe(int fd,int endOfFile);
int waitPid(int pid);
#endif