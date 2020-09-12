#ifndef _CALCULATOR_H
#define _CALCULATOR_H

#include <buffer.h>
#include <selfLib.h>
#include <stdint.h>
#include <stringFunctions.h>
#include <utils.h>

#define PRECISION_OF_DEC 7
#define TRUE 1
#define FALSE 0
#define BUFFERSIZE 80
#define FIRST '%'
#define EPSILON 0.00000001
#define ERROR -1
#define OK 6
#define PRECISION 4
#define NUMBER_MAX 300
#define CALC_MESSAGE ">Ingrese una expresion valida: "

typedef struct {
      double element;
      int type;
} calcElemT;

enum errorCode
{
   DIV_BY_ZERO = 0,
   INVALID_CHAR = 1,
   BRACKET_ERROR = 2,
   INV_EXP = 3
};

enum type{
   NUMBER=0,OPERATOR
};

void runCalculator();
#endif