// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <commands.h>
#include <phylo.h>
#include <selfLib.h>
#include <stringFunctions.h>
#include <test_util.h>

typedef struct {
    uint64_t pid;
    int state;
    void* sem;
} phylo_t;

enum states {
    THINKING = 0,
    HUNGRY,
    EATING,
    LEAVING
};

static void* lock;
static int remaining;
static int phy;

static phylo_t philosophers[MAX];
static int right(int i);
static int left(int i);
static void check(int i);
static void put_forks(int i);
static void take_forks(int i);
int philosopher(int argc, char* argv[]);
static void sleep();
static void removePhylo(int i);
static void addPhylo(int i);
static void printState();

int phylo(int argc, char* argv[]) {
    if ((lock = sem_open("sem", 1)) == NULL) {
        return -1;
    }
    remaining = 0;
    phy = 0;

    for (int i = 0; i < INITIAL_PHYLO; i++){
        sem_wait(lock);
        addPhylo(i);
        sem_post(lock);
    }

    int c;
    while ((c = getChar()) && remaining > 0) {
        printf("recibi %c\n", (char)c);
        if ((char)c == 'r' || (char)c == 'R')
            removePhylo(remaining-1);
        else if (((char)c == 'a' || (char)c == 'A') && remaining < MAX)
            addPhylo(remaining);
    }

    removePhylo(remaining-1);

    sem_close(lock);
    return 0;
}

static void removePhylo(int i){
    printf("Entering remove\n");

    sem_wait(lock);
    // printf("cr rem %d", i);
    remaining--;
    if(philosophers[i].state == EATING){
        philosophers[i].state = LEAVING;
        if(i!=0){
            check(i - 1);
            check(0);
        }
    }else{
        if(i!=0)
            check(i - 1);
    }
    
    printf("Remove %d\n", i);
    sem_close(philosophers[i].sem);
    _kill(philosophers[i].pid);
    sem_post(lock); 

}


static void addPhylo(int i) {
    char sem_name[10] = "phy";
    char number[10];
    char number2[10];
    uintToBase(remaining, (uint8_t*)number, 10);
    char* argv1[] = {"./philosopher", number};
    uintToBase(phy, (uint8_t*)number2, 10);
    strcat((uint8_t*)sem_name, (uint8_t*)number2);
    if ((philosophers[i].sem = sem_open(sem_name, 0)) != NULL) {
        char* argv1[] = {"./philosopher", number};
        philosophers[i].state = THINKING;
        philosophers[i].pid = addNewProcess(&philosopher, 2, argv1, BACKGROUND, NULL);
        remaining++;
        phy++;
    }
    printf("Add %d\n", i);
}

int philosopher(int argc, char* argv[]) {
    int idx = atoi((uint8_t*)argv[1]);
    while (1) {
        sleep();  // Thinking
        take_forks(idx);
        sleep();  // Eating
        put_forks(idx);
    }
    return 0;
}

static void take_forks(int i) {
    sem_wait(lock);
    // printf("cr%d ", i);
    philosophers[i].state = HUNGRY;
    check(i);
    
    // printf("lcrH%d ", i);
    sem_post(lock);
    sem_wait(philosophers[i].sem);
}

static void put_forks(int i) {
    sem_wait(lock);
    // printf("cr%d ", i);
    philosophers[i].state = THINKING;
    check(left(i));
    check(right(i));
    // printf("lcrT%d ", i);
    sem_post(lock);
}

/*
0 1 2 3 4
. E . . E  5
. E . E    4
. E . E    4
*/

static void check(int i) {
    int l, r;
    l = left(i);
    r = right(i);
    if (philosophers[i].state == HUNGRY && philosophers[l].state != EATING && philosophers[r].state != EATING) {
        philosophers[i].state = EATING;
        printf("filo %d: ", i);
        printState();
        sem_post(philosophers[i].sem);
    }
}

static int left(int i) {
    if (i == 0)
        return remaining - 1;
    else
        return i - 1;
}

static int right(int i) {
    if (i == remaining - 1)
        return 0;
    else
        return i + 1;
}

static void sleep() {
    long i = 0;
    long q = 10000000 + GetUniform(1000000);
    while (i < q) {
        i++;
    }
}

static void printState() {
    for(int i = 0; i < remaining;i++){
        if (philosophers[i].state == EATING)
            printf(" E ");
        else if(philosophers[i].state == HUNGRY)
            printf(" H ");
        else
            printf(" . ");
    }
    
    putChar('\n');
    
}