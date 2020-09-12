#ifndef _COMMANDS_H
#define _COMMANDS_H
#include <stdint.h>


void getMem(uint8_t * buffer,uint8_t * dir,uint8_t bytesToRead);

void _time(uint8_t * totalHour);

int _temperature(void);

void _information(uint8_t *string);

uint8_t _model();

void _invalidOpcodeException();

uint64_t * _inforeg();



#endif