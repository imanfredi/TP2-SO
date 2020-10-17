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
    enterCR(&creatingSem);

    sem_t* aux = semList->first;
    
    while (aux) {
        if (strcmp((uint8_t*)aux->name, (uint8_t*)name) == 0) {
            aux->proccessCount++;
            leaveCR(&creatingSem);
            return aux;
        }
        aux = aux->next;
    }
    
    aux = malloc2(sizeof(sem_t));
    if (aux == NULL) {
        leaveCR(&creatingSem);
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

    leaveCR(&creatingSem);
    
    return aux;
}
/*Si yo estoy bloqueando despierto al siguiente*/

int sem_close(sem_t* sem) {
    enterCR(&closingSem);

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
            leaveCR(&closingSem);
            return 0;
        }
        prev = curr;
        curr = curr->next;
    }

    leaveCR(&closingSem);

    return -1;
}

int sem_wait(sem_t* sem) {
    enterCR(&sem->lock);
    sem->value--;
    if (sem->value < 0) {
        enqueueBlock(sem, getCurrentPid());
        leaveCR(&sem->lock);
        block(getCurrentPid());
    } else {
        leaveCR(&sem->lock);
    }
    return 0;
}

int sem_post(sem_t* sem) {
    enterCR(&sem->lock);
    sem->value++;
    if (sem->firstWaiting != NULL) {
        int pid = dequeueBlock(sem);
        unblock(pid);  //unblock
    }

    leaveCR(&sem->lock);

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

static int dequeueBlock(sem_t* sem) {
    if (sem == NULL || sem->firstWaiting == NULL) {
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
    enterCR(&sem->lock);
    char * space="    ";
    uint8_t number[30];
    int len;
    printStringScreen((uint8_t*)sem->name,strlen((uint8_t*)sem->name),BLACK_WHITE);
    printStringScreen((uint8_t*)space,strlen((uint8_t*)space),BLACK_WHITE);

    len=uintToBase(sem->value,number,10);
    printStringScreen(number,len,BLACK_WHITE);
    printStringScreen((uint8_t*)space,strlen((uint8_t*)space),BLACK_WHITE);
    
    dumpProcessBlocked(sem->firstWaiting);
    
    printStringScreen((uint8_t*)space,strlen((uint8_t*)space),BLACK_WHITE);

    leaveCR(&sem->lock);
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

    enterCR(&sem->lock);
    sem->value = value;
    leaveCR(&sem->lock);
    return 0;

}