#ifndef _TERMINAL_H
#define _TERMINAL_H

#include <buffer.h>
#include <commands.h>
#include <selfLib.h>
#include <stdint.h>
#include <stringFunctions.h>
#include <utils.h>

#define BUFFERSIZE 80
#define CLEAR_SCREEN_CODE 252
#define COMMANDS 13
#define SIZE 15
#define DESC_SIZE 80
#define ARG_MAX 3
#define ARG_SIZE 18
#define TOREAD 32
#define REGISTERS 17
#define REG_NAME 4
#define SIZE_OF_REGISTER 16
#define SHELL_MESSAGE ">myshell:$ "

typedef struct
{
    uint8_t name[SIZE];
    void (*function)();
    uint8_t description[DESC_SIZE];
    uint8_t parameters;
} commandsT;


int runShell(int argc, char * argv[]);

#endif