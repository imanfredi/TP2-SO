#include <memoryManager.h>
#include <scheduler.h>
#include <screenDriver.h>
#include <stdint.h>
#include <lib.h>
#include <stringFunctionsKernel.h>

enum state { READY = 0,
             BLOCKED,
             KILLED };

#define STACK_SIZE 1024 * 4
#define INITIAL_PRIORITY 1
#define INIT_PROCESS 1
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
    uint8_t priority;
} pcb_t;

typedef struct node {
    pcb_t process;
    struct node *next;
} processNode;

typedef struct {
    processNode *first;
    processNode *last;
    uint64_t size;
} processQueue_t;

static uint64_t pidCounter;
static processNode * currentProcess;
static processQueue_t * processQueue;

static void initPCB(processNode *node, char *name, int (*function)(int, char **), uint64_t ppid);
static void initStackFrame(int (*function)(int, char **), int argc, char **argv, processNode *node);
static void loader2(int argc, char * argv[], int (*function)(int, char**));
static processNode * dequeueProcess();
static void enqueueProcess(processNode *process);
static int isEmpty();
static uint64_t newPid();
// int strlen(const char *s);
static void exit();
static void kill(uint64_t pid);
static void dumpProcess(processNode p);
static void dumpRSP(processNode p);

void initializeScheduler() {
    pidCounter = 0;
    processQueue = malloc2(sizeof(processQueue_t));
    processQueue->first = NULL;
    processQueue->last = NULL;
    currentProcess = NULL;
    processQueue->size = 0;
}

void finishScheduler() {
    free2(processQueue);
}

uint64_t addNewProcess(int (*function)(int, char **), int argc, char **argv) {
    processNode *node = (processNode *)malloc2(sizeof(processNode) + STACK_SIZE);
  //  printString("Post malloc: 0x", strlen("Post malloc: 0x"), 0x07); 
    //char buffer[9];
  //  int  len = uintToBaseWithLength(node, (uint8_t*)buffer, 16, 9);
//	printString(buffer,len+1,0x07);
   
    if (node == NULL)
        return -1;

    initPCB(node, argv[0], function, INIT_PROCESS);
    // printString("Arranco: ", strlen("Arranco: "), 0x07); 
    // printString(node->process.name, strlen(node->process.name), 0x07);
    // dumpRSP(*node);
    initStackFrame(function, argc, argv, node);
    enqueueProcess(node);
    return 0;
}

void initStackFrame(int (*function)(int, char **), int argc, char **argv, processNode *node) {
    t_stackFrame *stackFrame = (t_stackFrame *)(node->process.rsp);

    stackFrame->r15 = 8;
    stackFrame->r14 = 7;
    stackFrame->r13 = 6;
    stackFrame->r12 = 5;
    stackFrame->r11 = 4;
    stackFrame->r10 = 3;
    stackFrame->r9 = 2;
    stackFrame->r8 = 1;
    stackFrame->rsi = (uint64_t)argv;
    stackFrame->rdi = argc;
    stackFrame->rbp = 0;
    stackFrame->rdx = (uint64_t) function;
    stackFrame->rcx = 0;
    stackFrame->rbx = 0;
    stackFrame->rax = 0;
    stackFrame->rip = (uint64_t) loader2;
    stackFrame->cs = 0x8;
    stackFrame->eflags = 0x202;
    stackFrame->ss = 0x0;
    stackFrame->rsp = (uint64_t)node->process.rsp;
}

//Agregar next a si mismo. Llenar estructura pcb. Inicializar el stack con todos los valores correspondientes
/*
    0x000000                                                              0xFFFFFF
    ^processNode                                                 ^rbp                               
                            
         ^                        
    |processNode(... ,    rbp,rsp)     |
                            |
                            v
                 |    STACK      |    
    |  PROCESSNODE           STACKSIZE  regs     pushInterrupcion|
                                                             -   -
                                                            

                 */



void initPCB(processNode *node, char *name, int (*function)(int, char **), uint64_t ppid) {
    pcb_t * pcb = &(node->process);
    pcb->pid = newPid();
    pcb->ppid = ppid;
    pcb->name = name;
    pcb->rbp = (uint64_t)node + STACK_SIZE + sizeof(processNode) - sizeof(char *);
    pcb->rsp = (uint64_t)(pcb->rbp - sizeof(t_stackFrame));
    pcb->state = READY;
    pcb->entryPoint = function;
    pcb->priority = INITIAL_PRIORITY;

}
/*Si es null, es kernel el que esta generando la interrupcion del hlt
No debería encolar lo que tiene, solo sacar el proceso que esta en la cola
Desencolar el primero, encolar el que viene. Hacer el swapeo de rsp. Actualizar rsp, guardarlo en su estructura*/

uint64_t schedule(uint64_t rsp) {
    
    //encolar el que viene
    if (currentProcess == NULL) {
        if (!isEmpty())
            currentProcess = dequeueProcess();
        //else --> Hay que ver que pasa si no hay ningun proceso. Quien corre?
    } else {
        //switching context. Hay que quedarnos con las dos variables para actualizar los rsp
        
        currentProcess->process.rsp = rsp;
        enqueueProcess(currentProcess);
        do{
            currentProcess = dequeueProcess();
            if (currentProcess == NULL){
                // Poner dummy
            }
            else if(currentProcess->process.state == KILLED){
                free2(currentProcess);
            }    
        }while(currentProcess->process.state != READY);
    }

    // dumpProcess(*currentProcess);
    return currentProcess->process.rsp;
}

uint64_t newPid() {
    return ++pidCounter;
}

processNode * dequeueProcess() {
    
    if(isEmpty())
        return NULL;
    
    processNode * ans = processQueue->first;
    processQueue->size--;
    processQueue->first = ans->next;

    if (processQueue->last == ans){
        processQueue->last = NULL;
        processQueue->first = NULL;
    }

    return ans;     
}

void enqueueProcess(processNode *process) {
    if (isEmpty())
        processQueue->first = process;

    else
        processQueue->last->next = process;

    processQueue->last = process;
    processQueue->last->next=NULL;

    processQueue->size++;
}

int isEmpty() {
    return processQueue->size == 0;
}

void listProcess() {
    processNode *n = processQueue->first;
    char buffer[300];
    int len;

    /*nombre, ID, prioridad, stack y base pointer, foreground y cualquier otra variable que consideren
necesaria*/
    newLine();
    char *message = "nombre:   ID:   prioridad:   stack pointer:   basePointer:   foreground:   ";
    newLine();
    printString((uint8_t*)message, sizeof(message), BLACK_WHITE);

    do {
        len = 0;
        len += strcpy(buffer, n->process.name);
        len += strcpy(buffer + len, "   ");
        len += uintToBase(n->process.pid,(uint8_t*) buffer + len, 10);
        len += strcpy(buffer + len, "   ");
        len += uintToBase(n->process.priority, (uint8_t*)buffer + len, 10);
        len += strcpy(buffer + len, "   ");
        len += uintToBaseWithLength(n->process.rsp, (uint8_t*)buffer + len, 16, 17);
        len += strcpy(buffer + len, "   ");
        len += uintToBaseWithLength(n->process.rbp, (uint8_t*)buffer + len, 16, 17);
        len += strcpy(buffer + len, "   ");
        printString((uint8_t*)buffer, len, BLACK_WHITE);
        newLine();
        n = n->next;
    } while (n != processQueue->first);
}

void loader2(int argc, char * argv[], int (*function)(int, char**)){
    function(argc, argv);
    currentProcess->process.state = KILLED;
    kill(currentProcess->process.pid);
}

void kill(uint64_t pid){
    
    if (pid == currentProcess->process.pid){
        callTimerTick();
    }
}

static void dumpProcess(processNode p){
    
    printString("Name: ", 6, 0x07); 
    
    printString(p.process.name, strlen(p.process.name), 0x07);


    //printString("PID: ", strlen("PID: "), 0x07); 
    
    //printint(p.process.pid, strlen(p.process.pid), 0x07);

    printString("RSP: 0x", strlen("RSP: 0x"), 0x07); 
    char buffer[9];
    int  len = uintToBaseWithLength(currentProcess->process.rsp, (uint8_t*)buffer, 16, 9);
    printString(buffer,len+1,0x07);
   
    printString("RBP: 0x", strlen("RBP: 0x"), 0x07); 
    len = uintToBaseWithLength(currentProcess->process.rbp, (uint8_t*)buffer, 16, 9);
    printString(buffer,len+1,0x07);
}

static void dumpRSP(processNode p){
    printString("RSP: 0x", strlen("RSP: 0x"), 0x07); 
    char buffer[9];
    int  len = uintToBaseWithLength(currentProcess->process.rsp, (uint8_t*)buffer, 16, 9);
    printString(buffer,len+1,0x07);
}