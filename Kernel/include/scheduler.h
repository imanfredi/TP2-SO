#ifndef _SCHEDULER_H
#define _SCHEDULER_H

void initializeScheduler();
void addNewProcess(void (*function)(int, char **), int argc, char **argv);
void killProcess();
void finishScheduler();
void listProcess();

#endif