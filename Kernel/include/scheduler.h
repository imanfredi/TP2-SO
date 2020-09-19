#ifndef _SCHEDULER_H
#define _SCHEDULER_H

void initializeScheduler();
processNode * addNewProcess(void (*function)(int, char **), int argc, char **argv);
void killProcess();
void finishScheduler();


#endif