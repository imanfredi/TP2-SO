#ifndef _SCHEDULER_H
#define _SCHEDULER_H
#include <stdint.h>
void initializeScheduler();
uint64_t addNewProcess(int (*function)(int, char **), int argc, char **argv);
void killProcess();
void finishScheduler();
uint64_t listProcess();
uint64_t schedule(uint64_t rsp);

#endif