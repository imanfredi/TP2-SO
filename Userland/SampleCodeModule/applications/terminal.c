// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <terminal.h>
#include <phylo.h>

static char *command1[ARG_PROCESS];
static char *command2[ARG_PROCESS];
static char * arguments[MAX_ARGS];
static int argcP1 = 0;
static int argcP2 = 0;
static int fg = 0;
static int pipe = 0;
static int counter = 0;

static void readCommand(uint8_t *buffer, uint8_t *buffDim);
static int splitArgs(char * arguments[], int args);
static void cleanArgs();
static void sleep(int seconds);
static void memToStr(uint8_t *mem, uint8_t *memStr, uint8_t bytesToConvert);
static int loopProcess(int seconds);
static int checkParameters(int index, int argc);
static int getIndex(char *command);
static void waitPipe(int pipe, int p1,int p2);
static void addOneProcess();
static void addTwoProcess();


static int time(int argc, char *argv[]);
static int printmem(int argc, char *argv[]);
static int inforeg(int argc, char *argv[]);
static int information(int argc, char *argv[]);
static int temperature(int argc, char *argv[]);
static int DivideByZeroException(int argc, char *argv[]);
static int InvalidOpcodeException(int argc, char *argv[]);
static int help(int argc, char *argv[]);
static int ps(int argc, char *argv[]);
static int loop(int argc, char *argv[]);
static int kill(int argc, char *argv[]);
static int nice(int argc, char *argv[]);
static int block(int argc, char *argv[]);
static int test(int argc, char *argv[]);
static int loop(int argc, char *argv[]);
static int mem(int argc, char *argv[]);
static int sem(int argc, char *argv[]);
static int filter(int argc, char *argv[]);
static int cat(int argc, char *argv[]);
static int wc(int argc, char *argv[]);
static int unblock(int argc, char *argv[]);
static int pipeInfo(int argc, char*argv[]);


static commandsT commandVec[COMMANDS] = {
    {"help", &help, "Lista la informacion de los comandos disponibles.", 1},
    {"inforeg", &inforeg, "Informacion sobre el valor de todos los registros.", 1},
    {"printmem", &printmem, "Volcado de memoria de 32 bytes de a partir del puntero dado como argumento.", 2},
    {"time", &time, "Indica la hora actual.", 1},
    {"temperature", &temperature, "Informacion sobre la temperatura del procesador.", 1},
    {"information", &information, "Informacion sobre marca y modelo del procesador.", 1},
    {"testException0", &DivideByZeroException, "Realiza un testeo de la exception dividir por cero.", 1},
    {"testException6", &InvalidOpcodeException, "Realiza un testeo de la exception de Invalid Opcode.", 1},
    {"ps", &ps, "Imprime la informacion de los procesos corriendo actualmente", 1},
    {"block", &block, "Bloquea el proceso dado su id. Modo de uso \"block <PID>\"", 2},
    {"loop", &loop, "Imprime su ID con un saludo cada una determinada cantidad de segundos. Modo de uso \"loop <SECONDS>\"", 2},
    {"nice", &nice, "Cambia la prioridad de un proceso. Modo de uso \"nice <PID> <PRIORITY>\"", 3},
    {"kill", &kill, "Mata el proceso dado un id", 2},
    {"sem", &sem, "Imprime informacion de los semaforos", 1},
    {"mem", &mem, "Imprime informacion de la memoria", 1},
    {"test", &test, "Realiza un testeo. Modo de uso \"test <mm|prio|processes|sync|nosync>\"", 2},
    {"filter", &filter, "Filtra las vocales del input", 1},
    {"wc", &wc, "Cuenta la cantidad de lineas de input", 1},
    {"cat", &cat, "Imprime el stdin tal como la recibe", 1},
    {"unblock", &unblock, "Desbloquea el proceso dado su id. Modo de uso \"unblock <PID>\"", 2},
    {"pipe", &pipeInfo, "Imprime informacion de los pipes", 1},
    {"phylo",&phylo,"Simula el problema de los filosofos",1}};

static uint8_t registers[REGISTERS][REG_NAME] = {"RIP", "RSP", "RAX", "RBX", "RCX", "RDX", "RSI", "RBP",
                                                 "RDI", "R8 ", "R9 ", "R10", "R11", "R12", "R13", "R14", "R15"};


int runShell(int argc, char *argv[]) {
    uint8_t buffer[BUFFERSIZE];
    uint8_t buffDim = 0;
    needScreen();
    printString((uint8_t *)SHELL_MESSAGE);
    uint8_t c;
    while ((c = getChar()) != EXIT_CODE) {
        if (c != 0) {
            switch (c) {
                case '\n':
                    putChar('\n');
                    readCommand(buffer, &buffDim);
                    break;
                case CLEAR_SCREEN_CODE:

                    clear();
                    printString((uint8_t *)SHELL_MESSAGE);
                    printString(buffer);
                    break;
                case '\b':
                    if (buffDim > 0) {
                        removeBuffer(buffer, &buffDim);
                        putChar(c);
                    }
                    break;
                default:

                    if (addToBuffer(buffer, c, &buffDim, BUFFERSIZE)) {
                        putChar(c);
                    } else {
                        readCommand(buffer, &buffDim);
                    }
            }
        }
    }
    return 0;
}

static int help(int argc, char *argv[]) {
    for (int i = 0; i < COMMANDS; i++) {
        printf(" %c%s: %s\n", BULLET_POINT, commandVec[i].name, commandVec[i].description);
    }
    return 0;
}

static void readCommand(uint8_t *buffer, uint8_t *buffDim) {
    if (*buffDim > 0) {
        int args = strtok(' ',arguments,(char*)buffer,MAX_ARGS);
        if(splitArgs((char**)arguments,args) == 0){
            if (pipe == 1){

               addTwoProcess();
            }
            else{

                addOneProcess();
            }
        }else
            printString((uint8_t *)"Error en la entrada\n");

        cleanArgs();
    }
    cleanBuffer(buffer, buffDim);
    printString((uint8_t *)SHELL_MESSAGE);
}


static void addOneProcess(){
    int index = getIndex(command1[0]);
 
    if(index == -1)
        printString((uint8_t *)"Comando invalido\n");
    else{
        if (checkParameters(index, argcP1) != -1)
            addNewProcess(commandVec[index].function, argcP1, command1, fg, NULL);
        else
            printString((uint8_t *)"Numero erroneo de argumentos\n");
    }
}

static void addTwoProcess(){
    int index1 = getIndex(command1[0]);
    int index2 = getIndex(command2[0]);
    if(index1 == -1 || index2 == -1)
        printString((uint8_t *)"Comando invalido\n");
    else{
        if (checkParameters(index1, argcP1) != -1 && checkParameters(index2, argcP2) != -1) {
            uint8_t pipeName[10] = "p";
            uint8_t number[10];
            uintToBase(counter, number, 10);
            strcat((uint8_t*)pipeName, number);
            counter++;
            int aux = pipeOpen((char*)pipeName);
            if (aux != -1) {
                    int fd[2] = {aux, STDOUT};
                    int p2 = addNewProcess(commandVec[index2].function, argcP2, command2, BACKGROUND, fd); 
                    
                    fd[0] = STDIN;
                    fd[1] = aux;

                    int p1 = addNewProcess(commandVec[index1].function, argcP1, command1, fg, fd);
                    
                    waitPipe(aux,p1,p2);
                }
                else
                    printString((uint8_t *)"Error en la ejecucion\n");
        }
        else
            printString((uint8_t *)"Numero erroneo de argumentos\n");
    }
}



static void waitPipe(int pipe,int p1, int p2){
    
    if(fg==BACKGROUND){
        waitPid(p1);
    }
    writeInPipe(pipe,-1);
    
    waitPid(p2);

    closePipe(pipe);
}



static int getIndex(char *command) {
    int index = 0;
    while (strcmp(commandVec[index].name,(uint8_t*) command) != 0 && index < COMMANDS)
        index++;
    if (index >= COMMANDS)
        index = -1;

    return index;
}

static int checkParameters(int index, int argc) {
    if(index != -1 && commandVec[index].parameters == argc)
        return 0;
    return -1;
}

static int splitArgs(char * arguments[], int args) {
    
    
    char **aux = command1;
    int *argDim = &argcP1;
    int i = 0;
    while (i < args) {
        if (strcmp((uint8_t*)arguments[i],(uint8_t*)"|") == 0) {
            if (pipe==0) {
                aux = command2;
                argDim = &argcP2;
                pipe = 1;
            } else
                return -1;

        } else if (strcmp((uint8_t*)arguments[i],(uint8_t*)"&") == 0){
            if (fg == FOREGROUND)
                fg = BACKGROUND;
            else
                return -1;
        } else if (*argDim < ARG_PROCESS) {
            aux[(*argDim)++] = arguments[i];
        } else
            return -1;
        i++;    
    }

    return 0;
}

static void cleanArgs() {
    fg=FOREGROUND;
    argcP1 = 0;
    argcP2 = 0;
    pipe = 0;
}

int time(int argc, char *argv[]) {
    uint8_t totalHour[9];
    _time(totalHour);
    printf(" %cHora actual: %s\n", BULLET_POINT, totalHour);
    return 0;
}

static int printmem(int argc, char *argv[]) {
    if (argc < 2) {
        return -1;
    }

    printString((uint8_t *)"Volcado de memoria de 32 bytes: \n");
    char *dir = argv[1];
    if (isHexaDir(dir)) {
        uint8_t *address = (uint8_t *)stringHexaToNumber((uint8_t *)dir);
        uint8_t mem[TOREAD] = {0};
        uint8_t memStr[TOREAD * 2 + 1];
        uint8_t aux[4];
        getMem(mem, address, TOREAD);
        memToStr(mem, (uint8_t *)memStr, TOREAD);
        for (uint8_t i = 0, j = 0; i < TOREAD; i++) {
            if (i % (TOREAD / 4) == 0) {
                putChar('\n');
            }
            aux[0] = memStr[j++];
            aux[1] = memStr[j++];
            aux[2] = ' ';
            aux[3] = 0;
            printString(aux);
        }
        putChar('\n');
    } else {
        printString((uint8_t *)"No es una direcion hexadecimal de 64 bits\n");
    }
    return 0;
}

static void memToStr(uint8_t *mem, uint8_t *memStr, uint8_t bytesToConvert) {
    uint8_t aux[3];
    uint8_t i, j;
    for (i = 0, j = 0; i < bytesToConvert; i++) {
        uintToBase(mem[i], aux, 16);
        if (mem[i] < 16) {
            memStr[j++] = '0';
            memStr[j++] = aux[0];
        } else {
            memStr[j++] = aux[0];
            memStr[j++] = aux[1];
        }
    }
    memStr[j] = 0;
}

static int inforeg(int argc, char *argv[]) {
    printString((uint8_t *)"Impresion de registros: \n");
    uint8_t number[SIZE_OF_REGISTER + 1];
    uint64_t *registerValues = _inforeg();
    for (uint8_t i = 0; i < REGISTERS; i++) {
        uintToBaseWithLength(registerValues[i], number, 16, SIZE_OF_REGISTER + 1);
        printf(" %c %s: %s\n", BULLET_POINT, registers[i], number);
    }
    return 0;
}

static int ps(int argc, char *argv[]) {
    putChar('\n');
    _ps();
    putChar('\n');

    return 0;
}

static int loop(int argc, char *argv[]) {
    int seconds = atoi((uint8_t *)argv[1]);
    if (seconds < 0) {
        return -1;
    }
    return loopProcess(seconds);
}

static int loopProcess(int seconds) {
    int pid = getPid();

    while (1) {
        sleep(seconds);
        printf("\nHola! mi pid es: %d\n", pid);
    }
    return 0;
}

static void sleep(int seconds) {
    int totalTime = getSecondsElapsed() + seconds;
    while (getSecondsElapsed() <= totalTime);
}

static int nice(int argc, char *argv[]) {
    _nice(atoi((uint8_t *)argv[1]), atoi((uint8_t *)argv[2]));
    return 0;
}

static int block(int argc, char *argv[]) {
    _block(atoi((uint8_t *)argv[1]));
    return 0;
}

static int unblock(int argc, char *argv[]) {
    _unblock(atoi((uint8_t *)argv[1]));
    return 0;
}

static int kill(int argc, char *argv[]) {
    if (argc < 2)
        return -1;

    _kill(atoi((uint8_t *)argv[1]));
    return 0;
}

static int DivideByZeroException(int argc, char *argv[]) {
    int a = 1;
    int b = 0;
    a = a / b;
    return 0;
}

static int InvalidOpcodeException(int argc, char *argv[]) {
    _invalidOpcodeException();
    return 0;
}

static int information(int argc, char *argv[]) {
    uint8_t buff1[15];
    _information(buff1);
    uint8_t mod = _model();
    uint8_t fam = mod;
    mod = mod & 0xF;
    fam = (fam >> 4) & 0xF;
    printf(" %cMarca del procesador: %s\n", BULLET_POINT, buff1);
    uintToBase(mod, buff1, 10);
    printf(" %cModelo: %s\n", BULLET_POINT, buff1);
    uintToBase(fam, buff1, 10);
    printf(" %cFamily ID: %s\n", BULLET_POINT, buff1);
    return 0;
}

static int temperature(int argc, char *argv[]) {
    uint8_t str[4];
    uint8_t temp = _temperature();
    uintToBase(temp, str, 10);
    printf(" %cTemperatura del CPU: %s\n", BULLET_POINT, str);
    return 0;
}

static int mem(int argc, char *argv[]) {
    putChar('\n');
    memInfo();
    putChar('\n');
    return 0;
}
static int sem(int argc, char *argv[]) {
    putChar('\n');
    semInfo();
    putChar('\n');
    return 0;
}



static int test(int argc, char *argv[]) {
    uint8_t* parameter = (uint8_t*)argv[1];
    if(strcmp(parameter, (uint8_t*)"mm") == 0)
        return test_mm();
    else if(strcmp(parameter,(uint8_t*) "prio") == 0 )
        return test_prio();
    else if(strcmp(parameter,(uint8_t*) "sync") == 0)
        return test_sync();
    else if(strcmp(parameter,(uint8_t*) "nosync") == 0)
        return test_no_sync();
    else if(strcmp(parameter, (uint8_t*)"processes") == 0)
        return test_processes();
    
    printf("Test invalido");
    return -1;
}


static int cat(int argc, char *argv[]){
    
    int c;
    
    while ((c = getChar()) != EOF){
        putChar((char)c);
    }

    return 0;
}

static int wc(int argc, char *argv[]){
    uint64_t lines = 0;
    int c;

    while ((c = getChar()) != EOF){
        if((char)c == '\n')
            lines++;
    }
    uint8_t number[10];
    uintToBase(lines, number, 10);
    printString((uint8_t*)"Lines: ");
    printString(number);
    putChar('\n');
    return 0;
}

static int filter(int argc, char *argv[]){
    
    int c;
     while ((c = getChar()) != EOF){
        if(!isVowel((char)c))
            putChar((char)c);
    }

    return 0;
}

static int pipeInfo(int argc, char*argv[]){
    _pipeInfo();
    return 0;
}
