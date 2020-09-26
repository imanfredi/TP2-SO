#include <buffer.h>

int addToBuffer(uint8_t *buffer, char c, uint8_t *buffDim,uint8_t bufferMax)
{
    if (*buffDim < bufferMax - 1)
    {
        buffer[(*buffDim)++] = c;
        buffer[*buffDim] = 0;
        return 1;
    }
    return 0;
}

void cleanBuffer(uint8_t *buffer, uint8_t *buffDim)
{
    int i = 0;
    while (buffer[i] != 0)
    {
        buffer[i++] = 0;
    }
        *buffDim = 0;
}

void removeBuffer(uint8_t *buffer, uint8_t *buffDim)
{
    if(*buffDim>0)
    {
        buffer[--(*buffDim)] = 0;
    }
}