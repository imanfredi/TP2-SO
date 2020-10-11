#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#define SEM_MAX 30
#define FALSE 0
#define TRUE 1
#define MAX_LEN 25

typedef struct pNode {
    struct pNode * next;
    int pid;
} process_t;



typedef struct semNode {
    process_t * firstWaiting;
    process_t * lastWaiting;
    int value;
    char name[MAX_LEN];
    struct semNode * next;
    int lock;
    int proccessCount;
} sem_t;

typedef struct{
    sem_t * first;
}semList_t;

int initSemaphores();
sem_t * sem_open(char * name,int value);
int sem_close(sem_t * sem);
int sem_wait(sem_t * sem);
int sem_post(sem_t * sem);



#endif