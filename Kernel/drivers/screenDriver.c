#include <screenDriver.h>

static uint8_t *const initialMem = (uint8_t *)0xB8000;
static uint8_t *const endMem = (uint8_t *)0xB8FA0;

static uint64_t printString(const uint8_t *string, uint64_t size, uint8_t colour);


void initScreen()
{
    clearFullScreen();
    printString((uint8_t *)"PRESIONE ENTER PARA INICIAR ",44,BLACK_WHITE);
      
}

void clearFullScreen()
{
    uint8_t *initMem = initialMem;
    while (initMem < endMem)
    {
        *(initMem++) = ' ';
        *(initMem++) = BLACK_WHITE;
    }
}

void memcpyRange(uint8_t * dir1, uint8_t * dir2,uint64_t size){
        memcpy(dir1,dir2,size);
}

void putChar(uint8_t * pointer,uint8_t c,uint8_t colour){   
   if(pointer>=initialMem && pointer<endMem){
        *(pointer++)=c;
        *(pointer++)=colour;
   }
} 
void newLine(uint8_t * pointer)
{
    do
	{
		putChar(pointer,' ',BLACK_WHITE);
	} while ((uint64_t)(pointer - initialMem) % (TOTAL_COLS * 2) != 0);
    
}

uint64_t printString(const uint8_t *string, uint64_t size, uint8_t colour)
{
    uint64_t i = 0;
    uint8_t * mem = initialMem;
    while (string[i] != 0 && i < size)
    {   
        putChar(mem,string[i++],colour);
        mem+=2;

    }

    return i;
}