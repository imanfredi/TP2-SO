#ifndef _BUFFER_H
#define _BUFFER_H

#include <stdint.h>

#define BUFFER_MAX 80

int addToBuffer(uint8_t *buffer, char c, uint8_t *buffDim, uint8_t bufferMax);
void cleanBuffer(uint8_t *buffer, uint8_t *buffDim);
void removeBuffer(uint8_t *buffer, uint8_t *buffDim);

#endif