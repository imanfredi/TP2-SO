#ifndef _SCHEDULER_H
#define _SCHEDULER_H
#include <stdint.h>
void initializeScheduler();
void addNewProcess(void (*function)(int, char **), int argc, char **argv);
void killProcess();
void finishScheduler();
void listProcess();
uint64_t schedule(uint64_t rsp);

#endif