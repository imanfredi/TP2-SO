#ifndef _EXCEPTIONS_H
#define _EXCEPTIONS_H
#include <adminScreen.h>
#include <stdint.h>
#include <stringFunctionsKernel.h>
#define REG 17
#define REG_NAME 6
#define SIZE_OF_REGISTER 16
#define ZERO_EXCEPTION_ID 0
#define INVALID_OPERATION_CODE 6

typedef struct{
    char name[REG][REG_NAME];
    uint64_t values[REG];
}registerT;

void exceptionDispatcher(int exception);
void initExceptions(void* instructionPointer,void* stackPointer);
void saveRegisters(uint64_t* stackPointer);
#endif