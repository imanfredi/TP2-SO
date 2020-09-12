#include <adminScreen.h>

static screenT screenVec[MAXSCREENS];
static uint8_t currentScreen;
static uint8_t numberOfScreens=0;
static uint8_t colsPerScreen;
static uint8_t initialized=0;
static uint8_t *const initialMem = (uint8_t *)0xB8000;
static void scrollDown();
static void reloadInitialMemories();
static void splitScreen();

uint64_t startAppsVisual(){
    
        if(initialized==0){
            clearFullScreen();
            reloadInitialMemories();
            splitScreen();
            initialized=1;
        }else{
            for(int i=0;i<numberOfScreens;i++){
                currentScreen=i;
                clearScreen();
            }
        }
        
        return 1;
}


static void splitScreen()
{
    unsigned char splitter[] = {' ',SEPARATOR};
    uint8_t *line;
    for (int i = 0; i < TOTAL_ROWS; i++)
    {   
        line = (uint8_t*)(initialMem + 2 * i * TOTAL_COLS); 
        for(int j=1;j<numberOfScreens;j++){

            line+= 2 *colsPerScreen;
           
            *(line++) = (uint8_t)splitter[0];
            *(line++) = BLACK_WHITE;
            *(line++) = (uint8_t)splitter[1];
            *(line++) = BLACK_WHITE;
            *(line++) = (uint8_t)splitter[1];
            *(line++) = BLACK_WHITE;
            *(line++) = (uint8_t)splitter[0];
            *(line++) = BLACK_WHITE;
        }       
    }
    return;
}

int screenPartRequested(uint8_t place){

    if(numberOfScreens<MAXSCREENS && !screenVec[place].isReserved){
        colsPerScreen=(TOTAL_COLS-numberOfScreens*SEPARATOR_SIZE)/(numberOfScreens+1);
        currentScreen=numberOfScreens++;
        screenVec[currentScreen].currentCol = 0;
        screenVec[currentScreen].currentRow = 0;
        screenVec[currentScreen].isReserved=1;
        return 1;
    }
    return 0;
}

static void reloadInitialMemories(){
    for(int i=0;i<numberOfScreens;i++){
            screenVec[i].initialMem=initialMem +i *2*(colsPerScreen+SEPARATOR_SIZE);
            screenVec[i].currentMem=screenVec[i].initialMem;
    }    
}

uint8_t changeScreen(uint8_t screen)
{   
    if (screen<MAXSCREENS){
        currentScreen = screen;
        return 1;
    }
    return 0;
}

static void scrollDown()
{
    for (uint8_t i = 0; i < TOTAL_ROWS - 1; i++)
    {
        memcpyRange(screenVec[currentScreen].initialMem + (i * TOTAL_COLS * 2), screenVec[currentScreen].initialMem + (i + 1) * TOTAL_COLS * 2, colsPerScreen * 2);
    }

    uint8_t *lastLine = screenVec[currentScreen].initialMem + (TOTAL_ROWS - 1) * TOTAL_COLS * 2;
    screenVec[currentScreen].currentMem = lastLine;
    for (uint8_t i = 0; i < colsPerScreen; i++)
    {
        *(lastLine++)=' ';
        *(lastLine++)=BLACK_WHITE;

    }
    screenVec[currentScreen].currentCol = 0;
}

int clearScreen()
{
    if(numberOfScreens == 0){
        return 0;
    }
    screenVec[currentScreen].currentMem = screenVec[currentScreen].initialMem;
    screenVec[currentScreen].currentCol = 0;
    screenVec[currentScreen].currentRow = 0;
    for (int i = 0; i < colsPerScreen*TOTAL_ROWS; i++)
    {
        putCharScreen(' ',BLACK_WHITE);

    }
    screenVec[currentScreen].currentMem = screenVec[currentScreen].initialMem;
    screenVec[currentScreen].currentCol = 0;
    screenVec[currentScreen].currentRow = 0;
    return 1;
}

void newLineScreen()
 {   uint8_t aux=colsPerScreen - screenVec[currentScreen].currentCol;
    for (int i = 0; i < aux; i++){
        putCharScreen(' ',BLACK_WHITE);
    }
}

void putCharScreen(uint8_t c,uint8_t colour){

    putChar(screenVec[currentScreen].currentMem,c,colour);
    screenVec[currentScreen].currentMem+=2;

    screenVec[currentScreen].currentCol++;
    if (screenVec[currentScreen].currentCol == colsPerScreen)
    {
        if (screenVec[currentScreen].currentRow == TOTAL_ROWS - 1)
        {
            scrollDown();
        }
        else
        {
            screenVec[currentScreen].currentRow++;
            screenVec[currentScreen].currentMem = screenVec[currentScreen].initialMem + screenVec[currentScreen].currentRow * TOTAL_COLS * 2;
            screenVec[currentScreen].currentCol = 0;
        }
    }

}

void backSpaceScreen()
{

    if (screenVec[currentScreen].initialMem != screenVec[currentScreen].currentMem)
    {
        if (screenVec[currentScreen].currentCol == 0)
        {
            screenVec[currentScreen].currentRow--;
            screenVec[currentScreen].currentCol = colsPerScreen - 1;
            screenVec[currentScreen].currentMem = screenVec[currentScreen].initialMem + screenVec[currentScreen].currentRow * TOTAL_COLS * 2 + screenVec[currentScreen].currentCol * 2;
            *(screenVec[currentScreen].currentMem)=' ';
            *(screenVec[currentScreen].currentMem+1)=BLACK_WHITE;
        }
        else
        {
            screenVec[currentScreen].currentCol--;
            *(--screenVec[currentScreen].currentMem) = BLACK_WHITE;
            *(--screenVec[currentScreen].currentMem) = ' ';

        }
           
    }
}
uint64_t printStringScreen(const uint8_t *string, uint64_t size, uint8_t colour)
{
    uint64_t i = 0;
    while (string[i] != 0 && i < size)
    {
        putCharScreen(string[i++],colour);
    }
        
    return i;
}