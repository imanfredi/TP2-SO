#include <lib.h>
#include <memoryManager.h>
#include <scheduler.h>
#include <semaphore.h>
#include <stringFunctionsKernel.h>

#define MAX_LEN 25


static semList_t * semList;
static int creatingSem;
static int closingSem;
static void enqueueBlock(sem_t * sem, uint64_t pid);
static int dequeueBlock(sem_t * sem);

int initSemaphores() {
    semList = malloc2(sizeof(semList_t));
    creatingSem = 0;
    closingSem = 0 ;

    if(semList == NULL)
        return -1;
    
    semList->first = NULL;
    return 0;
}

sem_t * sem_open(char* name,int value) {

    enterCR(&creatingSem);    
    
    sem_t * aux = semList->first; 
  
    while(aux){
        if(strcmp((uint8_t*)aux->name,(uint8_t*) name) == 0){
            leaveCR(&creatingSem);
            return aux;
        }
        aux=aux->next;
    }

    aux = malloc2(sizeof(sem_t));
    if(aux == NULL){
        leaveCR(&creatingSem);
        return NULL;
    }
    
    aux->firstWaiting = NULL;
    aux->lastWaiting = NULL;
    aux->lock = 0;
    aux->value = value;
    strncpy((uint8_t*)aux->name,(uint8_t*)name, MAX_LEN-1);
    sem_t * second = semList->first;
    semList->first = aux;
    aux->next = second;

    leaveCR(&creatingSem);

    return aux;
}
/*Si yo estoy bloqueando despierto al siguiente*/

int sem_close(sem_t * sem){
    
    enterCR(&closingSem);

    sem_t * prev = semList->first;
    sem_t * curr = prev;
    
    while(curr){
        
        if(sem == curr){
           sem->proccessCount--;
            
            if(sem->proccessCount == 0){
                prev->next=curr->next;
                free2(sem);
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


int sem_wait(sem_t * sem){
    
    enterCR(&sem->lock);
    sem->value--;
    if(sem->value < 0){
        enqueueBlock(sem,getCurrentPid());
    
        leaveCR(&sem->lock);

        block(getCurrentPid());
    }
    else{
        leaveCR(&sem->lock);
    }
    return 0;
}



int sem_post(sem_t * sem){
    
    enterCR(&sem->lock);
    sem->value++;
    if(sem->firstWaiting != NULL){
        int pid = dequeueBlock(sem);
        block(pid); //unblock
    }
   
    leaveCR(&sem->lock);
    
    return 0;
}

static void enqueueBlock(sem_t * sem, uint64_t pid){
    
    process_t * p = malloc2(sizeof(process_t));

    p->pid = pid;
    p->next = NULL;

    if (sem->firstWaiting == NULL){
        sem->firstWaiting = p;
        sem->lastWaiting = p;
    }else{
        process_t * aux = sem->lastWaiting;
        aux->next = p;
        sem->lastWaiting = p;
    }  
}

static int dequeueBlock(sem_t * sem){

    if(sem == NULL || sem->firstWaiting==NULL){
        return -1;
    }
    
    process_t * curr = sem->firstWaiting;
    int pid = curr->pid;
    sem->firstWaiting = curr->next;
    if(sem->firstWaiting==NULL)
        sem->lastWaiting=NULL;
        
    free2(curr);
    return pid;
}

/*

//static int addOwner(sem_t * sem, uint64_t pid);

typedef struct semNode {
    process_t * firstWaiting;
    process_t * firstOwner;
    process_t * blockingProcess;
    int value;
    char* name;
    sem_t * next;
} sem_t;


sem_t * sem_open(char* name) {
    sem_t * aux = semList->first; 
    int found;
    while(aux){
        if(strcmp(aux->name, name) == 0){
            if(addOwner(aux, getCurrentPid()) == -1)
                return NULL;
            return aux;
        }
        aux=aux->next;
    }

    aux = malloc2(sizeof(sem_t));
    if(aux == NULL)
        return NULL;
    
    if(addOwner(aux, getCurrentPid()) == -1)
        return NULL;
                
    aux->firstWaiting = NULL;
    aux->value = 1;
    aux->name = malloc2(strlen(name)+1);
    strcpy(aux->name,name);
    sem_t * second = semList->first;
    semList->first = aux;
    aux->next = second;

    return aux;
}

static int addOwner(sem_t * sem, uint64_t pid){
    process_t * aux = sem->firstOwner;
       
    while(aux != NULL && aux->next != NULL){
        if(aux->pid == pid)
            return 0 ;
        aux = aux->next;
    }

    process_t * newOwner = malloc2(sizeof(process_t));
    if(newOwner == NULL)
        return -1;
    
    newOwner->pid = pid;
    newOwner->next = NULL;
    
    if(aux==NULL)
        sem->firstOwner = newOwner;
    
    else
        aux->next = newOwner;
    
    return 0;
}


int sem_close(sem_t * sem){
    sem_t * aux = semList->first;
    sem = (sem_t*)sem;
    
    while(aux){
        if(sem == aux){

            

        }

        aux=aux->next;
    }

    return -1;
}
   
   //remove owner si es que está. Removerlo tambien de los bloqueados si es que estaba ahí.  Si no quedan mas hacer free del name y del semaforo

    shell -> a y b

        


a blocked
b blockeador
c 

*/
