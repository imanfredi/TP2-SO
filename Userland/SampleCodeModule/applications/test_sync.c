// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <commands.h>
#include <selfLib.h>
#include <stringFunctions.h>
#include <test_util.h>

#define TOTAL_PAIR_PROCESSES 2
#define SEM_ID "sem"

int64_t global;  //shared memory

void slowInc(int64_t* p, int64_t inc) {
    int64_t aux = *p;
    aux += inc;
    _yield();
    *p = aux;
}

int inc(int argc, char*argv[]){

    int sem = atoi((uint8_t*)argv[1]);
    int value = atoi((uint8_t*)argv[2]);
    int N = atoi((uint8_t*)argv[3]);
    void * aux;
    uint64_t i;
    
    if (sem && (aux=sem_open(SEM_ID, 1)) == NULL) {
        printf("ERROR OPENING SEM\n");
        return -1;
    }

    for (i = 0; i < N; i++) {
        if (sem){
            sem_wait(aux);
        }
        slowInc(&global, value);
        if (sem){
            sem_post(aux);
        }
    }

    if (sem){
        sem_close(aux);
    }

    if(sem)
        printf("SYNC Final value: %d\n", global);
    else
        printf("NO SYNC Final value: %d\n", global);

    return 0;
}

int test_sync() {
    uint64_t i;

    global = 0;
    
    printf("CREATING PROCESSES...(WITH SEM)\n");
    
    char * argv1[]={"inc","1","1","100000"};
    char * argv2[]={"inc","1","-1","100000"};


    for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
        addNewProcess(&inc, 4, argv1, BACKGROUND,NULL);
        addNewProcess(&inc, 4,argv2, BACKGROUND,NULL);
    }
    return 0;
}

int test_no_sync() {
    uint64_t i;

    global = 0;

    printf("CREATING PROCESSES...(WITHOUT SEM)\n");

    char * argv1[]={"inc","0","1","1000000"};
    char * argv2[]={"inc","0","-1","1000000"};

    for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
        addNewProcess(&inc, 4, argv1, BACKGROUND,NULL);
        addNewProcess(&inc, 4,argv2, BACKGROUND,NULL);
    }
    return 0;
}
