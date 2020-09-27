#ifndef _ADM_SCREEN_H
#define _ADM_SCREEN_H

#include <stdint.h>
#include <screenDriver.h>

#define SEPARATOR 177
#define MAXSCREENS 4
#define SEPARATOR_SIZE 4

uint64_t startAppsVisual();
int screenPartRequested();
uint8_t changeScreen(uint8_t screen);
int clearScreen();
void newLineScreen();
void putCharScreen(uint8_t c,uint8_t colour);
void backSpaceScreen();
uint64_t printStringScreen(const uint8_t *string, uint64_t size, uint8_t colour);

enum screen
{
    SCREENA=0,
    SCREENB,
    SCREENC,
    SCREEND,
    SCREENE
};

typedef struct
{
    uint8_t * initialMem;
    uint8_t * currentMem;
    uint8_t currentCol;
    uint8_t currentRow; 
    uint8_t isReserved;
}screenT;

#endif