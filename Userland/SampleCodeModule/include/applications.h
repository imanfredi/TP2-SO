#ifndef _APPLICATIONS_H
#define _APPLICATIONS_H

#include <buffer.h>
#include <selfLib.h>
#include <stdint.h>

#define MAXAPPLICATIONS 4

typedef struct app
{   uint8_t buffer[BUFFER_MAX];
    uint8_t bufferSize;
    void (*execFunction)(uint8_t * buff,uint8_t *buffsize);
    uint8_t id;
    uint8_t * message;
} appT;

int runApp();
int addApplication(void * func,char * message);

#endif