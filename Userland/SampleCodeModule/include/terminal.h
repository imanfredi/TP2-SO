#ifndef _TERMINAL_H
#define _TERMINAL_H

#include <buffer.h>
#include <commands.h>
#include <selfLib.h>
#include <stdint.h>
#include <stringFunctions.h>
#include <test_util.h>
#include <utils.h>

#define BUFFERSIZE 80
#define CLEAR_SCREEN_CODE 252
#define COMMANDS 17
#define SIZE 25
#define FALSE 0
#define TRUE 1
#define DESC_SIZE 100
#define ARG_MAX 4
#define ARG_SIZE 18
#define TOREAD 32
#define REGISTERS 17
#define REG_NAME 4
#define SIZE_OF_REGISTER 16
#define SHELL_MESSAGE ">myshell:$ "

typedef struct
{
    uint8_t name[SIZE];
    int (*function)(int argc, char *argv[]);
    uint8_t description[DESC_SIZE];
    uint8_t parameters;
} commandsT;

int runShell(int argc, char *argv[]);

#endif