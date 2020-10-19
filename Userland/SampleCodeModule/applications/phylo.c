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
};

enum isLocked{
    LOCKED = 0,
    UNLOCKED,
};

static void* lock;
static int remaining;
static int phy;

static phylo_t philosophers[MAX];
static int locked[MAX] = {0};
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
static void killRemaining();

int phylo(int argc, char* argv[]) {
    if ((lock = sem_open("sem", 1)) == NULL) {
        return -1;
    }
    remaining = 0;
    phy = 0;

    for (int i = 0; i < INITIAL_PHYLO; i++)
        addPhylo(i);

    int c;
    while ((c = getChar()) != EOF && remaining > 0) {
        if ((char)c == 'r' || (char)c == 'R')
            removePhylo(remaining - 1);
        else if (((char)c == 'a' || (char)c == 'A') && remaining < MAX)
            addPhylo(remaining);
    }

    killRemaining();

    sem_close(lock);
    return 0;
}

static void removePhylo(int i) {
    sem_wait(lock);
    if(locked[i] == LOCKED){
        sem_post(lock);
        removePhylo(i); // Hay que esperar a que haga su sem_post
        return;
    }else{
        if (philosophers[i].state == EATING && i != 0) {    
            sem_close(philosophers[i].sem);
            _kill(philosophers[i].pid);
            remaining--;
            printf("Removed from the table %d\n", i);
            check(i - 1);
            check(0);
        }
        else{
            sem_close(philosophers[i].sem);
            _kill(philosophers[i].pid);
            remaining--;
            printf("Removed from the table %d\n", i);
        }
    }
    
    sem_post(lock);
}

static void addPhylo(int i) {
    sem_wait(lock);
    char sem_name[10] = "phy";
    char number[10];
    char number2[10];
    uintToBase(remaining, (uint8_t*)number, 10);
    uintToBase(phy, (uint8_t*)number2, 10);
    strcat((uint8_t*)sem_name, (uint8_t*)number2);
    if ((philosophers[i].sem = sem_open(sem_name, 0)) != NULL) {
        char* argv1[] = {"./philosopher", number};
        philosophers[i].state = THINKING;
        philosophers[i].pid = addNewProcess(&philosopher, 2, argv1, BACKGROUND, NULL);
        remaining++;
        phy++;
    }
    printf("Add to the table %d\n", i);
    sem_post(lock);
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
    locked[i] = LOCKED;
    sem_wait(lock);
    philosophers[i].state = HUNGRY;
    check(i);
    sem_post(lock);
    locked[i] = UNLOCKED;

    sem_wait(philosophers[i].sem);
}

static void put_forks(int i) {
    locked[i] = LOCKED;
    sem_wait(lock);
    philosophers[i].state = THINKING;    
    check(left(i));
    check(right(i));
    sem_post(lock);
    locked[i] = UNLOCKED;    
}

static void check(int i) {
    int l, r;
    l = left(i);
    r = right(i);
    if (philosophers[i].state == HUNGRY && philosophers[l].state != EATING && philosophers[r].state != EATING) {
        philosophers[i].state = EATING;
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
    for (int i = 0; i < remaining; i++) {
        if (philosophers[i].state == EATING)
            printf(" E ");
        else
            printf(" . ");
    }

    putChar('\n');
}

static void killRemaining(){

    while (remaining > 0){
        sem_close(philosophers[remaining-1].sem);
        _kill(philosophers[remaining-1].pid);
        remaining--;
    }
    
    
}

/**H ----  H E
H ----  . E
. ----  H E
. ----  . E

E ---- E H
E ---- H H 
E ---- . H 
H ---- E H 
H ---- H H 
H ---- . H 
. ---- E H 
. ---- H H 
. ---- . H 
 
E ---- E . 
E ---- H . 
E ---- . . 
H ---- E . 
H ---- H . 
H ---- . . 
. ---- E . 
. ---- H . 
. ---- . . 

 * 
 * 
 * 
 * 
 * 
 * 
*/