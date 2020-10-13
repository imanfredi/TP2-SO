#include <pipe.h>
#include <semaphore.h>
#include <stdint.h>
#include <stringFunctionsKernel.h>
#include <adminScreen.h>

typedef struct {
    int idxWrite;
    int idxRead;
    int dim;
    char buffer[SIZE];
    char name[NAME_MAX];
    void* semRead;
    void* semWrite;
    int processUsing;
    int isActive;
} pipe_t;

static void* creatingPipe;
static void* closingPipe;

static pipe_t pipeArray[PIPES_MAX];
static int size;
static int dimPipes;

int initPipes() {

    if ((creatingPipe = sem_open("CreatingPipe", 1)) == NULL)
        return -1;
    
    if ((closingPipe = sem_open("ClosingPipe", 1)) == NULL)
        return -1;
    
    size = 0;
    dimPipes = PIPES_MAX;
    if(pipeOpen(STDIN_NAME) == -1 || pipeOpen(STDOUT_NAME) == -1)
        return -1;
    return 0;
}

int pipeOpen(char* name) {
    sem_wait(creatingPipe);
    int firstInactive = -1;

    for (int i = 0; i < dimPipes; i++) {
        if (pipeArray[i].isActive && strcmp((uint8_t*)pipeArray[i].name,(uint8_t*) name) == 0) {
            pipeArray[i].processUsing++;
            sem_post(creatingPipe);
            return i;
        } else if (firstInactive == -1)
            firstInactive = i;
    }
    if (firstInactive == -1) {
        sem_post(creatingPipe);
        return -1;
    }
    pipeArray[firstInactive].dim = SIZE;
    pipeArray[firstInactive].idxWrite = 0;
    pipeArray[firstInactive].idxRead = 0;

    pipeArray[firstInactive].processUsing = 1;
    pipeArray[firstInactive].isActive = 1;

    char buffer[NAME_MAX + 1];
    strncpy((uint8_t*)buffer, (uint8_t*)name, NAME_MAX - 1);
    int len = strlen((uint8_t*)buffer);
    buffer[len + 1] = 0;

    buffer[len] = 'R';
    if ((pipeArray[firstInactive].semRead = sem_open(buffer, 0)) == NULL)
        return -1;

    buffer[len] = 'W';
    if ((pipeArray[firstInactive].semWrite = sem_open(buffer, pipeArray[firstInactive].dim)) == NULL)  //le pongo la cantidad que entran en el buffer que es la cantidad maxima que puede escribir antes de bloquearse
        return -1;

    strncpy((uint8_t*)pipeArray[firstInactive].name, (uint8_t*)name, NAME_MAX - 1);
    sem_post(creatingPipe);
    return firstInactive;
}

int closePipe(int fd) {
    sem_wait(closingPipe);

    pipeArray[fd].processUsing--;
    if (pipeArray[fd].processUsing <= 0) {
        pipeArray[fd].isActive = 0;
        if (sem_close(pipeArray[fd].semRead) == -1)
            return -1;
        if (sem_close(pipeArray[fd].semWrite) == -1)
            return -1;
    }

    sem_post(closingPipe);
    return 0;
}

int writePipe(int fd, char c) {
    sem_wait(pipeArray[fd].semWrite);
    if (pipeArray[fd].idxWrite == pipeArray[fd].dim)
        pipeArray[fd].idxWrite = 0;

    pipeArray[fd].buffer[pipeArray[fd].idxWrite++] = c;
    sem_post(pipeArray[fd].semRead);

    return 0;
}

char readPipe(int fd) {
    sem_wait(pipeArray[fd].semRead);

    if (pipeArray[fd].idxRead == pipeArray[fd].dim)
        pipeArray[fd].idxRead = 0;

    char c = pipeArray[fd].buffer[pipeArray[fd].idxRead++];

    sem_post(pipeArray[fd].semWrite);

    return c;
}

int writePipeString(int fd, char* buffer) {
    int i;
    for (i = 0; buffer[i] != 0; i++)
        writePipe(fd, buffer[i]);
    return i;
}
/*
int pipeInfo(){
    
    for (int i = 0; i < dimPipes; i++){
        if(pipeArray[i].isActive){
           // dumpPipe(pipeArray[i]);
            newLineScreen();
        }
    }

    return 0;
    
}

void dumpPipe(pipe_t pipe){
    pipe.semWrite
}
*/