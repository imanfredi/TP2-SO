// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <lib.h>
#include <memoryManager.h>
#include <scheduler.h>
#include <semaphore.h>
#include <stringFunctionsKernel.h>
#include <adminScreen.h>

#define MAX_LEN 25

static semList_t* semList;
static int creatingSem;
static int closingSem;
static void enqueueBlock(sem_t* sem, uint64_t pid);
static int dequeueBlock(sem_t* sem);
static void dumpProcessBlocked(process_t * process);
void dumpSem(sem_t * sem);

int initSemaphores() {
    semList = malloc2(sizeof(semList_t));
    creatingSem = 0;
    closingSem = 0;

    if (semList == NULL)
        return -1;

    semList->first = NULL;
    return 0;
}
sem_t* sem_open(char* name, int value) {
    while(_xchg(&creatingSem,1)!=0);
    
    sem_t* aux = semList->first;
    
    while (aux) {
        if (strcmp((uint8_t*)aux->name, (uint8_t*)name) == 0) {
            aux->proccessCount++;
            _xchg(&creatingSem,0);
            return aux;
        }
        aux = aux->next;
    }
    
    aux = malloc2(sizeof(sem_t));
    if (aux == NULL) {
        _xchg(&creatingSem,0);
        return NULL;
    }

    aux->firstWaiting = NULL;
    aux->lastWaiting = NULL;
    aux->lock = 0;
    aux->value = value;
    aux->proccessCount=1;
    strncpy((uint8_t*)aux->name, (uint8_t*)name, MAX_LEN - 1);
    sem_t* second = semList->first;
    semList->first = aux;
    aux->next = second;

    _xchg(&creatingSem,0);
    
    return aux;
}
/*Si yo estoy bloqueando despierto al siguiente*/

int sem_close(sem_t* sem) {
    while(_xchg(&closingSem,1)!=0);

    sem_t* prev = semList->first;
    sem_t* curr = prev;

    while (curr) {
        if (sem == curr) {
            sem->proccessCount--;

            if (sem->proccessCount == 0) {
                if(prev == curr)
                    semList->first = curr->next;
                else
                    prev->next = curr->next;
                free2(curr);
            }
            _xchg(&closingSem,0);
            return 0;
        }
        prev = curr;
        curr = curr->next;
    }

    _xchg(&closingSem,0);

    return -1;
}

int sem_wait(sem_t* sem) {
    while(_xchg(&sem->lock,1)!=0);
    sem->value--;
    if (sem->value < 0) {
        enqueueBlock(sem, getCurrentPid());
        _xchg(&sem->lock,0);
        block(getCurrentPid());
    } else {
        _xchg(&sem->lock,0);
    }
    return 0;
}

int sem_post(sem_t* sem) {
    while(_xchg(&sem->lock,1)!=0);
    sem->value++;
    if (sem->firstWaiting != NULL) {
        int pid = dequeueBlock(sem);
        unblock(pid);
    }

    _xchg(&sem->lock,0);

    return 0;
}

static void enqueueBlock(sem_t* sem, uint64_t pid) {
    process_t* p = malloc2(sizeof(process_t));

    p->pid = pid;
    p->next = NULL;

    if (sem->firstWaiting == NULL) {
        sem->firstWaiting = p;
        sem->lastWaiting = p;
    } else {
        process_t* aux = sem->lastWaiting;
        aux->next = p;
        sem->lastWaiting = p;
    }
}


static int dequeueBlock(sem_t * sem){

    if(sem == NULL || sem->firstWaiting==NULL){
        return -1;
    }

    process_t* curr = sem->firstWaiting;
    int pid = curr->pid;
    sem->firstWaiting = curr->next;
    if (sem->firstWaiting == NULL)
        sem->lastWaiting = NULL;

    free2(curr);
    return pid;
}

int semInfo(){
    sem_t * aux = semList->first;
    char * message="Nombre del semaforo:   Estado:    PID BLOQUEADOS";
    printStringScreen((uint8_t*)message,strlen((uint8_t*)message),BLACK_WHITE);
    newLineScreen();
    while(aux){
        dumpSem(aux);
        newLineScreen();
        aux=aux->next;
    }
    return 0;
}



void dumpSem(sem_t * sem){
    while(_xchg(&sem->lock,1)!=0);
    char * space="    ";
    uint8_t number[30];

    printStringScreen((uint8_t*)sem->name,strlen((uint8_t*)sem->name),BLACK_WHITE);
    printStringScreen((uint8_t*)space,strlen((uint8_t*)space),BLACK_WHITE);

    itoa(sem->value, number, 10);
    printStringScreen(number,strlen(number),BLACK_WHITE);
    printStringScreen((uint8_t*)space,strlen((uint8_t*)space),BLACK_WHITE);
    
    dumpProcessBlocked(sem->firstWaiting);
    
    printStringScreen((uint8_t*)space,strlen((uint8_t*)space),BLACK_WHITE);

    _xchg(&sem->lock,0);
}


static void dumpProcessBlocked(process_t * process){

    char * space="    ";
    uint8_t number[10];
    

    while(process){
        int len = uintToBase(process->pid,number,10);
        printStringScreen(number,len,BLACK_WHITE);
        printStringScreen((uint8_t*)space,strlen((uint8_t*)space),BLACK_WHITE);
    
        process=process->next;
    }

}

int changeValue(sem_t * sem,int value){

    while(_xchg(&sem->lock,1)!=0);
    sem->value = value;
    _xchg(&sem->lock,0);
    return 0;

}