// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <adminScreen.h>
#include <keyboardDriver.h>
#include <lib.h>
#include <memoryManager.h>
#include <rtc.h>
#include <scheduler.h>
#include <semaphore.h>
#include <stdint.h>
#include <systemCallDispatcher.h>
#include <time.h>
#include <pipe.h>
#include <stringFunctionsKernel.h>

/*Se declara el tipo que devuelven y luego lo que reciben. Si el parentesis esta vacio, significa que recibe cualquier cosa*/
static uint64_t writeSyscall(Register_t *registers);
static uint64_t read(Register_t *registers);
static uint64_t clear(Register_t *registers);
static uint64_t swapScreen(Register_t *registers);
static uint64_t readMem(Register_t *registers);
static uint64_t time(Register_t *registers);
static uint64_t information(Register_t *registers);
static uint64_t temperature(Register_t *registers);
static uint64_t screenRequest(Register_t *registers);
static uint64_t newProcess(Register_t *registers);
static uint64_t ps(Register_t *registers);
static uint64_t nicePriority(Register_t *registers);
static uint64_t blockProcess(Register_t *registers);
static uint64_t killProcess(Register_t *registers);
static uint64_t getSeconds(Register_t *registers);
static uint64_t getPid(Register_t *registers);
static uint64_t yieldSyscall(Register_t *registers);
static uint64_t mallocSyscall(Register_t *registers);
static uint64_t freeSyscall(Register_t *registers);
static uint64_t semOpenSyscall(Register_t *registers);
static uint64_t semCloseSyscall(Register_t *registers);
static uint64_t semPostSyscall(Register_t *registers);
static uint64_t semWaitSyscall(Register_t *registers);
static uint64_t semInfoSyscall(Register_t *registers);
static uint64_t memInfoSyscall(Register_t *registers);
static uint64_t changeValueSyscall(Register_t *registers);
static int writeInStdout(char * buffer,uint64_t len,uint8_t colour);
static uint64_t pipeOpenSyscall(Register_t *registers);
static uint64_t closePipeSyscall(Register_t *registers);
static uint64_t writeInPipeSyscall(Register_t * registers);
static uint64_t waitSyscall(Register_t * registers);
static uint64_t unblockProcess(Register_t *registers);
static uint64_t pipeInfoSyscall(Register_t * registers);


static uint64_t (*syscalls[FUNCTIONS])(Register_t *) = {&read, &writeSyscall, &clear, &swapScreen, &readMem, &time, &information,
                                                        &temperature, &cpuModel, &getRegisters, &screenRequest, &startAppsVisual,
                                                        &newProcess, &ps, &blockProcess, &nicePriority, &killProcess, &getSeconds,
                                                        &getPid, &yieldSyscall, &mallocSyscall, &freeSyscall, &semOpenSyscall, &semCloseSyscall,
                                                        &semPostSyscall, &semWaitSyscall, &semInfoSyscall,&memInfoSyscall,&changeValueSyscall,
                                                        &pipeOpenSyscall,&closePipeSyscall,&writeInPipeSyscall,&waitSyscall,&unblockProcess,
                                                        &pipeInfoSyscall};

uint64_t systemCallDispatcher(Register_t *parameters) {
    uint64_t output = -1;

    if (parameters->rax < FUNCTIONS) {
        output = syscalls[parameters->rax](parameters);
    }
    return output;
}
/*
Funcion getChar:
*/
static uint64_t read(Register_t *registers) {
    int aux =  getCurrentStdin();
    if(aux == STDIN)
        return getCharacter();
    else
        return readPipe(aux);

}

/*
Funcion write:
    Recibe por parametros en rdi el file descriptor, el buffer con el string  en rsi y la cantidad de caracteres en rdx
*/

static uint64_t writeSyscall(Register_t *registers) {
    char * buffer = (char *)registers->rsi;
    return (uint64_t)write(buffer,(int)registers->rdx,(int)registers->rdi);   
}

uint64_t write(char * buffer,int len,int std){
        int fd = getCurrentStdout();
        if(fd == STDOUT){
            uint8_t colour;
            if(std == 1)
                colour = BLACK_WHITE;
            else
                colour = BLACK_RED;
            return writeInStdout(buffer,len,colour);
        }
        else
            return writePipeString(buffer,len,fd);
}

static int writeInStdout(char * buffer,uint64_t len,uint8_t colour){
    int i;
    for (i = 0; i < len; i++) {
        if (buffer[i] == '\n') 
            newLineScreen();
        else if (buffer[i] == '\b')
            backSpaceScreen();
        else
            putCharScreen(buffer[i], colour);
    }        
    return i;
}

static uint64_t clear(Register_t *registers) {
    return clearScreen();
}

static uint64_t swapScreen(Register_t *registers) {
    return changeScreen(registers->rdi);
}
static uint64_t readMem(Register_t *registers) {
    uint8_t *buffer = (uint8_t *)registers->rdi;
    uint8_t *pointer = (uint8_t *)registers->rsi;
    int bytesToRead = registers->rdx;
    int i;
    for (i = 0; i < bytesToRead; i++) {
        buffer[i] = pointer[i];
    }
    return i;
}

static uint64_t time(Register_t *registers) {
    return buildTime((uint8_t *)registers->rdi);
}
static uint64_t information(Register_t *registers) {
    return (uint64_t)cpuVendor((uint8_t *)(registers->rdi));
}

static uint64_t temperature(Register_t *registers) {
    return _temperature();
}

static uint64_t screenRequest(Register_t *registers) {
    return screenPartRequested();
}

static uint64_t newProcess(Register_t *registers) {
    return addNewProcess((int (*)(int, char **))registers->rdi, (int)registers->rsi, (char **)registers->rdx, registers->rcx,(int*)registers->r8);
}

static uint64_t ps(Register_t *registers) {
    return listProcess();
}

static uint64_t blockProcess(Register_t *registers) {
    return block((uint64_t)registers->rdi);
}

static uint64_t nicePriority(Register_t *registers) {
    return nice((uint64_t)registers->rdi, (uint64_t)registers->rsi);
}

static uint64_t killProcess(Register_t *registers) {
    return kill((uint64_t)registers->rdi);
}

static uint64_t getPid(Register_t *registers) {
    return getCurrentPid();
}

static uint64_t getSeconds(Register_t *registers) {
    return seconds_elapsed();
}

static uint64_t yieldSyscall(Register_t *registers) {
    return yield();
}
static uint64_t mallocSyscall(Register_t *registers) {
    return (uint64_t)malloc2((unsigned int)registers->rdi);
}

static uint64_t freeSyscall(Register_t *registers) {
    free2((void *)registers->rdi);
    return 0;
}

static uint64_t semOpenSyscall(Register_t *registers) {  //22
    return (uint64_t)sem_open((char *)registers->rdi, (int)registers->rsi);
}
static uint64_t semCloseSyscall(Register_t *registers) {  //23
    return sem_close((sem_t *)registers->rdi);
}
static uint64_t semPostSyscall(Register_t *registers) {  //24
    return sem_post((sem_t *)registers->rdi);
}
static uint64_t semWaitSyscall(Register_t *registers) {  //25
    return sem_wait((sem_t *)registers->rdi);
}

static uint64_t semInfoSyscall(Register_t *registers) {  //26
    return semInfo();
}

static uint64_t memInfoSyscall(Register_t *registers) {  //27
    return memInfo();
}

static uint64_t changeValueSyscall(Register_t *registers) {  //28
    return changeValue((sem_t *)registers->rdi, (int)registers->rsi);
}

static uint64_t pipeOpenSyscall(Register_t *registers) {  //29
    return pipeOpen((char*)registers->rdi);
}


static uint64_t closePipeSyscall(Register_t *registers) {  //30
    return closePipe((int)registers->rdi);
}

static uint64_t writeInPipeSyscall(Register_t * registers){ //31
    return writePipe((int)registers->rdi,(char)registers->rsi);
}

static uint64_t waitSyscall(Register_t * registers){//32
    return wait(registers->rdi);
}

static uint64_t unblockProcess(Register_t *registers) { //33
    return unblock((uint64_t)registers->rdi);
}

static uint64_t pipeInfoSyscall(Register_t * registers){ //34
    return pipeInfo();

}