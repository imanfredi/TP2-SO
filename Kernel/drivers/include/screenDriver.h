#ifndef _SCREEN_DRIVER_H
#define _SCREEN_DRIVER_H

#include <stdint.h>
#include <lib.h>
#define TOTAL_COLS 80
#define TOTAL_ROWS 25
#define BLACK_WHITE 0x07
#define BLACK_RED 0x04


void initScreen();
void clearFullScreen();
void newLine();
void putChar(uint8_t * pointer,uint8_t c,uint8_t colour);
void memcpyRange(uint8_t * dir1, uint8_t * dir2,uint64_t size);



#endif