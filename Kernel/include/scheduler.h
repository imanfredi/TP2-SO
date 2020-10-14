#ifndef _SCHEDULER_H
#define _SCHEDULER_H
#include <stdint.h>
#define STACK_SIZE 1024 * 4
#define INITIAL_PRIORITY 1
#define INIT_PROCESS 1
#define FOREGROUND 0
#define BACKGROUND 1
#define SIZE_REGISTER 16
#define QUANTUM 1
#define STDIN 0
#define STDOUT 1
#define MAX_PRIORITY 10
#define NULL 0

typedef struct {
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;
    uint64_t rip;
    uint64_t cs;
    uint64_t eflags;
    uint64_t rsp;
    uint64_t ss;
} t_stackFrame;

typedef struct {
    uint64_t pid;
    uint64_t ppid;
    char *name;
    uint64_t rsp;
    uint64_t rbp;
    uint32_t state;
    void *entryPoint;
    uint32_t stdin;
    uint32_t stdout;
    uint64_t priority;
    uint64_t slotsLeft;
    uint64_t execution;  //se guarda si corre en foreground o background
} pcb_t;

typedef struct node {
    pcb_t process;
    struct node *next;
} processNode;

typedef struct {
    processNode *first;
    processNode *last;
    uint64_t size;
    uint64_t ready;
} processQueue_t;

void initializeScheduler();
uint64_t addNewProcess(int (*function)(int, char **), int argc, char **argv, uint64_t execution,int fd[]);
uint64_t kill(uint64_t pid);
uint64_t loop(void);
uint64_t nice(uint64_t pid, uint64_t priority);
uint64_t block(uint64_t pid);
void finishScheduler();
uint64_t listProcess();
uint64_t schedule(uint64_t rsp);
uint64_t block(uint64_t pid);
uint64_t getCurrentPid();
uint64_t yield();
int getCurrentStdin();
int getCurrentStdout();
int wait(uint64_t pid);

#endif