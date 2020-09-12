#ifndef _SYSTEM_CALL_DISPATCHER_H
#define _SYSTEM_CALL_DISPATCHER_H
#define FUNCTIONS 12
#define BLACK_WHITE 0x07
#define BLACK_RED 0x04

#include <stdint.h>

/* Estructura de registros */
typedef struct
{
    uint64_t rax, rdi, rsi, rdx, r8, r9, r10;

} Register_t;


uint64_t systemCallDispatcher(Register_t *parameters);

#endif