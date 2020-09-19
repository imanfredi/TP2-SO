#include <scheduler.h>
#include <stdint.h>

enum state { READY = 0,
             BLOCKED,
             KILLED };
             
#define STACK_SIZE 1024 * 4
#define INITIAL_PRIORITY 1
#define INIT_PROCESS 1

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
    uint8_t priority;
} pcb_t;

typedef struct {
    pcb_t process;
    processNode *next;
} processNode;

typedef struct {
    processNode *first;
    uint8_t number_of_process;
} processQueue_t;



static uint64_t pidCounter;
static processQueue_t * processQueue; 

void initializeScheduler() {
    pidCounter = 0;
    processQueue = malloc2(sizeof(processQueue_t));
}


void finishScheduler(){
    free(processQueue);
}

//Agregar next a si mismo. Llenar estructura pcb. Inicializar el stack con todos los valores correspondientes
/*
    0x000000                                                              0xFFFFFF
    |  -----                            regs     pushInterrupcion|
    ^processNode                                 
                                 ^                        

    |processNode(... ,    rbp,rsp)     |
                            |
                            v
                 |    STACK      |
    */

processNode * addNewProcess(void (*function)(int, char **), int argc, char **argv) {
    processNode * node = (processNode *)malloc2(sizeof(processNode) + STACK_SIZE);
    if (node == NULL)
        return 0;

    initPCB(node, argv[0], function, INIT_PROCESS);
    initStackFrame(function, argc, argv, node);
    
    
    /*

    |pcb next                                             stackframe|
    
    */

}

void initStackFrame(void (*function)(int, char **), int argc, char **argv, processNode *node) {
    t_stackFrame *stackFrame = (t_stackFrame *)(node->process.rsp);

    stackFrame->r15 = 10;
    stackFrame->r14 = 9;
    stackFrame->r13 = 8;
    stackFrame->r12 = 7;
    stackFrame->r11 = 6;
    stackFrame->r10 = 5;
    stackFrame->r9 = 4;
    stackFrame->r8 = 3;
    stackFrame->r9 = 2;
    stackFrame->r8 = 1;
    stackFrame->rsi = argv;
    stackFrame->rdi = argc;
    stackFrame->rbp = 0;
    stackFrame->rdx = 0;
    stackFrame->rcx = 0;
    stackFrame->rbx = 0;
    stackFrame->rax = 0;
    stackFrame->rip = function;
    stackFrame->cs = 0x8;
    stackFrame->eflags = 0x202;
    stackFrame->ss = 0x0;
    stackFrame->rsp = (uint64_t)node->process.rsp;
}

void initPCB(processNode *node, const char *name, void (*function)(int, char **), uint64_t ppid) {
    pcb_t *pcb = &(node->process);

    pcb->pid = newPid();
    pcb->ppid = ppid;
    pcb->name = name;
    pcb->rbp = (uint64_t)(node + STACK_SIZE + sizeof(processNode) - sizeof(char *));
    pcb->rsp = (uint64_t)(pcb->rbp - sizeof(t_stackFrame));
    pcb->state = READY;
    pcb->entryPoint = function;
    pcb->priority = INITIAL_PRIORITY;
}

int nextProcess() {
}

int64_t newPid() {
    return ++pidCounter;
}

void killProcess(){
    //free del stack del proceso
}
