// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <applications.h>
#include <selfLib.h>
#include <stringFunctions.h>

static appT application[MAXAPPLICATIONS];
static uint8_t cantApp = 0;
static uint8_t currentApp = 0;

static void swapApplication();

int runApp() {
    while (getChar() != '\n')
        ;
    initVisualEnvironment();
    for (int i = 0; i < cantApp; i++) {
        swapApplication(i);
        printString(application[i].message);
    }
    while (1) {
        application[currentApp].execFunction(application[currentApp].buffer, &application[currentApp].bufferSize);
        swapApplication();
    }
}

void swapApplication() {
    currentApp = (currentApp + 1) % cantApp;
    nextScreen(currentApp);
}

int addApplication(void *func, char *message) {
    if (cantApp < MAXAPPLICATIONS) {
        application[cantApp].execFunction = func;
        application[cantApp].id = cantApp;
        application[cantApp].message = (uint8_t *)message;
        cleanBuffer(application[cantApp].buffer, &application[cantApp].bufferSize);
        needScreen(application[cantApp].id);
        currentApp = cantApp++;
        return 1;
    }
    return 0;
}