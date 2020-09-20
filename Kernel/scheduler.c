#include <memoryManager.h>
#include <scheduler.h>
#include <stdint.h>
#include <adminScreen.h>

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

typedef struct node{
    pcb_t process;
    struct node * next;
} processNode;

typedef struct {
    processNode *first;
    processNode *last;
    uint64_t size;
} processQueue_t;

static uint64_t pidCounter;
static processNode * currentProcess;
static processQueue_t *processQueue;


void initializeScheduler() {
    pidCounter = 0;
    processQueue = malloc2(sizeof(processQueue_t));
    processQueue->first = NULL;
    processQueue->last = NULL;
    currentProcess = NULL;
    processQueue->size = 0;
}

void finishScheduler() {
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

void addNewProcess(void (*function)(int, char **), int argc, char **argv) {
    processNode *node = (processNode *)malloc2(sizeof(processNode) + STACK_SIZE);
    if (node == NULL)
        return 0;

    initPCB(node, argv[0], function, INIT_PROCESS);
    initStackFrame(function, argc, argv, node);

    enqueueProcess(node);
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


void nextProcess() {
    //encolar el que viene
    if (currentProcess == NULL) {
        if (!isEmpty)
            currentProcess = dequeueProcess();
        //else --> Hay que ver que pasa si no hay ningun proceso. Quien corre?
    } else {
        //switching context. Hay que quedarnos con las dos variables para actualizar los rsp
        swap();
        enqueueProcess(currentProcess);
    }
}

void swap(processNode *){

}

    int64_t newPid() {
    return ++pidCounter;
}

void killProcess(uint64_t idToKill) {
    //free del stack del proceso
    //Desencolarlos
    while (current->process.id == idToKill) {
    }
    dequeueProcess();
}



processNode * dequeue() {
}

void enqueueProcess(processNode *process) {
    if (isEmpty())
        processQueue->first = process;

    else
        processQueue->last->next = process;

    processQueue->last = process;
    processQueue->size++;
}

int isEmpty() {
    return processQueue.size == 0;
}

int exit(){

}


void listProcess() {
    processNode * n=processQueue->first;
    char buffer[300];
    int len=0;

    /*nombre, ID, prioridad, stack y base pointer, foreground y cualquier otra variable que consideren
necesaria*/
    newLineScreen();
    char * message= "nombre:   ID:   prioridad:   stack pointer:   basePointer:   foreground:   ";
    newLineScreen();
    printStringScreen(message,sizeof(message),BLACK_WHITE);

    do{
        len+=strcpy(buffer,n->process.name);
        len+=strcpy(buffer+len,"   ");
        len+= uintToBase(n->process.pid,buffer+len,10);
        len+=strcpy(buffer+len,"   ");
        len+=uintToBase(n->process.priority,buffer+len,10);
        len+=strcpy(buffer+len,"   ");
        len+=uintToBaseWithLength(n->process.rsp,buffer+len,16,17);
        len+=strcpy(buffer+len,"   ");
        len+=uintToBaseWithLength(n->process.rbp,buffer+len,16,17);
        len+=strcpy(buffer+len,"   ");
        //len+=
        printStringScreen(buffer,len,BLACK_WHITE);
        n = n->next;
    }while(n != processQueue->first)
    
    
}

/*Esto habria que moverlo de aca*/
uint32_t uintToBaseWithLength(uint64_t value, uint8_t *buffer, uint32_t base, uint8_t size) {
      uint8_t *p = buffer + size - 1;
      *p-- = 0;
      uint32_t digits = 0;
      //Calculate characters for each digit
      do {
            uint64_t remainder = value % base;
            *p-- = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
            digits++;
      } while (value /= base);

      while (buffer <= p) {
            *p-- = '0';
      }

      return digits;
}
/*Esta tabien de aca hay que volarla */

int strlen(const uint8_t *s)
{

    int i = 0;

    while (s[i] != 0)
        i++;
   
    return i;

}

uint32_t uintToBase(uint64_t value, uint8_t *buffer, uint32_t base)
{
    uint8_t *p = buffer;
    uint8_t *p1, *p2;
    uint32_t digits = 0;

    //Calculate characters for each digit
    do
    {
        uint32_t remainder = value % base;
        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
        digits++;
    } while (value /= base);

    // Terminate string in buffer.
    *p = 0;

    //Reverse string in buffer.
    p1 = buffer;
    p2 = p - 1;
    while (p1 < p2)
    {
        uint8_t tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }

    return digits;
}

int strcpy(uint8_t dest[], const uint8_t source[])
{
    int i = 0;
    while (source[i] != '\0')
    {
        dest[i] = source[i];
        i++;
    }
    dest[i]=0;

    return i;;
}
