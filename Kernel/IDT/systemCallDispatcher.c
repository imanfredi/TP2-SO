#include <stdint.h>
#include <systemCallDispatcher.h>
#include <keyboardDriver.h>
#include <rtc.h>
#include <lib.h>
#include <adminScreen.h>
#include <scheduler.h>


/*Se declara el tipo que devuelven y luego lo que reciben. Si el parentesis esta vacio, significa que recibe cualquier cosa*/
static uint64_t write(Register_t * registers);
static uint64_t read(Register_t * registers);
static uint64_t clear(Register_t * registers);
static uint64_t swapScreen(Register_t * registers);
static uint64_t readMem(Register_t *registers);
static uint64_t time(Register_t *registers);
static uint64_t information(Register_t *registers);
static uint64_t temperature(Register_t *registers);
static uint64_t screenRequest(Register_t * registers);
static uint64_t newProcess(Register_t * registers);

static uint64_t (*syscalls[FUNCTIONS])(Register_t *) = {&read, &write, &clear, &swapScreen,&readMem,&time,&information,&temperature,&cpuModel,&getRegisters,&screenRequest,&startAppsVisual,&newProcess};

uint64_t systemCallDispatcher(Register_t *parameters)
{
    uint64_t output = -1;

    if (parameters->rax >= 0 && parameters->rax < FUNCTIONS)
    {
        output = syscalls[parameters->rax](parameters);
    }
    return output;
}
/*
Funcion getChar:
*/
static uint64_t read(Register_t *registers){
        return getCharacter();
}
/*
Funcion write:
    Recibe por parametros en rdi el file descriptor, el buffer con el string  en rsi y la cantidad de caracteres en rdx
*/
static uint64_t write(Register_t *registers)
{   
    uint8_t colour;
    char * buffer = (char*)registers->rsi;
    if(registers->rdi == 1){
        colour = BLACK_WHITE;
    }
    else
    {
        colour = BLACK_RED;
    }
    int i;
    for ( i = 0; i < registers -> rdx; i++)
    {
        if(buffer[i] == '\n'){
            newLineScreen();
        }
        else if(buffer[i] == '\b'){
            backSpaceScreen();
        }
        else{
            putCharScreen(buffer[i],colour);
        }
    }
    return i;
}

static uint64_t clear(Register_t *registers){
    return clearScreen();
}

static uint64_t swapScreen(Register_t *registers){
    return changeScreen(registers->rdi);
}
static uint64_t readMem(Register_t *registers){
    uint8_t *buffer = (uint8_t *)registers->rdi;
    uint8_t *pointer = (uint8_t *)registers->rsi;
    int bytesToRead = registers->rdx;
    int i;
    for (i = 0; i < bytesToRead; i++) {
        buffer[i] = pointer[i];
    }
      return i;
}

static uint64_t time(Register_t *registers){
    return buildTime((uint8_t*)registers->rdi);
}
static uint64_t information(Register_t *registers){
    return (uint64_t)cpuVendor((uint8_t*)(registers->rdi));
}   

static uint64_t temperature(Register_t *registers){
    return _temperature();
}

static uint64_t screenRequest(Register_t * registers){
    return screenPartRequested((uint8_t)registers->rdi);
}

static uint64_t newProcess(Register_t * registers){
    return addNewProcess((int (*)(int, char **))registers->rdi,(int)registers->rsi,(char**)registers->rdx);
}
