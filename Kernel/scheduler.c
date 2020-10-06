#include <adminScreen.h>
#include <interrupts.h>
#include <lib.h>
#include <memoryManager.h>
#include <scheduler.h>
#include <stdint.h>
#include <stringFunctionsKernel.h>

enum state { READY = 0,
             BLOCKED,
             KILLED };

static uint64_t pidCounter;
static processNode *currentProcess;
static processQueue_t *processQueue;
static processNode *dummy;

static void initPCB(processNode *node, char *name, int (*function)(int, char **), uint64_t ppid);
static void initStackFrame(int (*function)(int, char **), int argc, char **argv, processNode *node);
static void loader2(int argc, char *argv[], int (*function)(int, char **));
static processNode *dequeueProcess();
static void enqueueProcess(processNode *process);
static int isEmpty();
static uint64_t newPid();
static void printProcessInfo(processNode *n);
static processNode *findNode(uint64_t pid);
static int dummyProcess(int argc, char *argv[]);
static char **copyArgv(char **buff, char **argv, int argc);


void initializeScheduler() {
    pidCounter = 0;
    processQueue = malloc2(sizeof(processQueue_t));
    processQueue->first = NULL;
    processQueue->last = NULL;
    currentProcess = NULL;
    char *argv[] = {"dummyProcess"};
    addNewProcess(&dummyProcess, 1, argv);
    dummy = dequeueProcess();
    processQueue->size = 0;
}

static int dummyProcess(int argc, char *argv[]) {
    while (1) {
        _hlt();
    }
    return 0;
}

void finishScheduler() {
    free2(processQueue);
}

uint64_t addNewProcess(int (*function)(int, char **), int argc, char *argv[]) {
    processNode *node = (processNode *)malloc2(sizeof(processNode) + STACK_SIZE);

    if (node == NULL)
        return -1;

    initPCB(node, argv[0], function, INIT_PROCESS);
    argv = copyArgv((char **)((uint64_t)node + sizeof(processNode)), argv, argc);
    initStackFrame(function, argc, argv, node);
    enqueueProcess(node);
    return node->process.pid;
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
    stackFrame->rdx = (uint64_t)function;
    stackFrame->rcx = 0;
    stackFrame->rbx = 0;
    stackFrame->rax = 0;
    stackFrame->rip = (uint64_t)loader2;
    stackFrame->cs = 0x8;
    stackFrame->eflags = 0x202;
    stackFrame->ss = 0x0;
    stackFrame->rsp = (uint64_t)node->process.rsp;
}

void initPCB(processNode *node, char *name, int (*function)(int, char **), uint64_t ppid) {
    pcb_t *pcb = &(node->process);
    pcb->pid = newPid();
    pcb->ppid = ppid;
    pcb->name = name;
    pcb->rbp = (uint64_t)node + STACK_SIZE + sizeof(processNode) - sizeof(char *);
    pcb->rsp = (uint64_t)(pcb->rbp - sizeof(t_stackFrame));
    pcb->state = READY;
    pcb->entryPoint = function;
    pcb->priority = INITIAL_PRIORITY;
    pcb->slotsLeft = INITIAL_PRIORITY * QUANTUM;
}
/*Si es null, es kernel el que esta generando la interrupcion del hlt
No debería encolar lo que tiene, solo sacar el proceso que esta en la cola
Desencolar el primero, encolar el que viene. Hacer el swapeo de rsp. Actualizar rsp, guardarlo en su estructura*/

uint64_t schedule(uint64_t rsp) {
    //encolar el que viene
    if (currentProcess == NULL) {  //si es el primer proceso que se corre
        if (!isEmpty()) {
            currentProcess = dequeueProcess();
            currentProcess->process.slotsLeft = QUANTUM * currentProcess->process.priority;
        }
    } else {
        //switching context. Hay que quedarnos con las dos variables para actualizar los rsp
        currentProcess->process.rsp = rsp;

        if (currentProcess->process.slotsLeft <= 0) {
                enqueueProcess(currentProcess);
            do {
                currentProcess = dequeueProcess();
                if (currentProcess->process.state == KILLED) {
                    free2(currentProcess);
                }

            } while (currentProcess != NULL && currentProcess->process.state != READY);

            if (currentProcess != NULL)
                currentProcess->process.slotsLeft = QUANTUM * currentProcess->process.priority;
        }
    }
    if (currentProcess == NULL)
        currentProcess = dummy;
    else {
        currentProcess->process.slotsLeft--;
    }

    return currentProcess->process.rsp;
}

uint64_t newPid() {
    return ++pidCounter;
}

processNode *dequeueProcess() {
    if (isEmpty())
        return NULL;

    processNode *ans = processQueue->first;
    processQueue->size--;
    processQueue->first = ans->next;

    if (processQueue->last == ans) {
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
    processQueue->last->next = NULL;

    processQueue->size++;
}

static int isEmpty() {
    return processQueue->size == 0;
}

uint64_t loop(void) {
    return 0;
}

uint64_t block(uint64_t pid) {
    if (currentProcess->process.pid == pid) {
        currentProcess->process.state = BLOCKED;
        callTimerTick();
    } else {
        processNode *current = findNode(pid);
        if (current != NULL) {
            if (current->process.state == BLOCKED)
                current->process.state = READY;
            else if (current->process.state == READY)
                current->process.state = BLOCKED;
            return 0;
        }
    }
    listProcess();
    return -1;
}

uint64_t yield(){
    currentProcess->process.slotsLeft=0;
    callTimerTick();
    return 0;
}

uint64_t nice(uint64_t pid, uint64_t priority) {
    if (priority > MAX_PRIORITY)
        priority = MAX_PRIORITY;
    else if (priority <= 0) {
        priority = INITIAL_PRIORITY;
    }

    if (currentProcess->process.pid == pid)
        currentProcess->process.priority = priority;
    else {
        processNode *node = findNode(pid);
        if (node == NULL)
            return -1;

        node->process.priority = priority;
    }

    return 0;
}

uint64_t listProcess() {
    processNode *n = processQueue->first;
    char *message = "NAME   ID:   PRIORITY:   SP:   BP:   foreground:   ";
    printStringScreen((uint8_t *)message, strlen((uint8_t *)message), BLACK_WHITE);
    newLineScreen();

    for (n = processQueue->first; n != NULL; n = n->next) {
        printProcessInfo(n);
        newLineScreen();
    }

    printProcessInfo(currentProcess);
    return 0;
}

static void printProcessInfo(processNode *n) {
    uint8_t number[10];
    uint8_t registers[SIZE_REGISTER + 1];
    uint64_t len = 0;

    printStringScreen((uint8_t *)n->process.name, (uint64_t)strlen((uint8_t *)n->process.name), 0x07);

    printStringScreen((uint8_t *)"   ", strlen((uint8_t *)"   "), 0x07);

    len = uintToBase(n->process.pid, (uint8_t *)number, 10);
    printStringScreen(number, len, 0x07);
    printStringScreen((uint8_t *)"   ", strlen((uint8_t *)"   "), 0x07);

    len = uintToBase(n->process.priority, number, 10);
    printStringScreen(number, len, 0x07);
    printStringScreen((uint8_t *)"   ", strlen((uint8_t *)"   "), 0x07);

    len = uintToBaseWithLength(n->process.rsp, registers, SIZE_REGISTER, SIZE_REGISTER + 1);
    printStringScreen(registers, SIZE_REGISTER + 1, 0x07);
    printStringScreen((uint8_t *)"   ", strlen((uint8_t *)"   "), 0x07);

    len = uintToBaseWithLength(n->process.rbp, registers, SIZE_REGISTER, SIZE_REGISTER + 1);
    printStringScreen(registers, SIZE_REGISTER + 1, 0x07);
    printStringScreen((uint8_t *)"   ", strlen((uint8_t *)"   "), 0x07);
}

void loader2(int argc, char *argv[], int (*function)(int, char **)) {
    function(argc, argv);
    kill(currentProcess->process.pid);
}

uint64_t kill(uint64_t pid) {
    if (pid == currentProcess->process.pid) {
        currentProcess->process.state = KILLED;
        callTimerTick();
    }

    processNode *node = findNode(pid);
    if (node != NULL){
        node->process.state = KILLED;
        return 0;
    }
        listProcess();
        return -1;

}

static processNode *findNode(uint64_t pid) {
    processNode *curr = processQueue->first;
    while (curr != NULL) {
        if (curr->process.pid == pid) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

uint64_t getCurrentPid() {
    return currentProcess->process.pid;
}

static char **copyArgv(char **buff, char **argv, int argc) {
    char *dest = (char *)(buff + argc);

    for (uint32_t i = 0; i < argc; i++) {
        buff[i] = dest;
        for (char *aux = argv[i]; *aux; aux++, dest++) {
            *dest = *aux;
        }
        *dest = 0;
        dest++;
    }

    return buff;
}
