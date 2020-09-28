#include <terminal.h>


static uint8_t argDim =0;
static uint8_t arg1[ARG_SIZE]={0};
static uint8_t arg2[ARG_SIZE]={0};
static uint8_t arg3[ARG_SIZE]={0};
static uint8_t *arguments[ARG_MAX] = {arg1,arg2,arg3};
static void readCommand(uint8_t * buffer,uint8_t *buffDim);
static void splitArgs(uint8_t * buffer,uint8_t * buffDim);
static void cleanArgs();
static void time();
static void printmem(uint8_t * dir);
static void inforeg();
static void information();
static void temperature();
static void DivideByZeroException();
static void InvalidOpcodeException();
static void help();
static void ps();
static void loop(uint8_t * seconds);
static void kill(uint8_t * pid);
static void nice(uint8_t * pid,uint8_t * priority);
static void block(uint8_t * pid);
static void memToStr(uint8_t *mem, uint8_t *memStr, uint8_t bytesToConvert);
static int loopProcess(int argc, char *argv[]);
static void loop(uint8_t * seconds);
static void sleep(int seconds);

static commandsT commandVec[COMMANDS] = {
        {"help", &help, "Lista la informacion de los comandos disponibles.", 1},
        {"inforeg", &inforeg, "Informacion sobre el valor de todos los registros.", 1},
        {"printmem", &printmem, "Volcado de memoria de 32 bytes de a partir del puntero dado como argumento.", 2},
        {"time", &time, "Indica la hora actual.", 1},
        {"temperature", &temperature, "Informacion sobre la temperatura del procesador.", 1},
        {"information", &information, "Informacion sobre marca y modelo del procesador.", 1},
        {"testException0",&DivideByZeroException, "Realiza un testeo de la exception dividir por cero.",1},
        {"testException6",&InvalidOpcodeException, "Realiza un testeo de la exception de Invalid Opcode.",1},
        {"ps", &ps, "Imprime la informacion de los procesos corriendo actualmente", 1},
        {"block",&block,"Bloquea el proceso dado su id",2},
        {"loop",&loop,"Imprime su ID con un saludo cada una determinada cantidad de segundos",2},
        {"nice", &nice, "Cambia la prioridad de un proceso. Modo de uso \"nice <PID> <PRIORITY>\"",3},
        {"kill",&kill,"Mata el proceso dado un id",2} };

static uint8_t registers[REGISTERS][REG_NAME] = {"RIP","RSP","RAX", "RBX", "RCX", "RDX", "RSI", "RBP", 
                                            "RDI","R8 ", "R9 ", "R10", "R11", "R12", "R13", "R14", "R15"};

int runShell(int argc,char * argv[] ){   
        
           
        uint8_t buffer[BUFFERSIZE];
        uint8_t buffDim=0;
        needScreen();

        printString((uint8_t*)SHELL_MESSAGE);
        
        uint8_t c;
        
        while ((c = getChar()) != EXIT_CODE)
        {

            if (c != 0)
            {   
                switch (c)
                {
                    case '\n':
                                putChar('\n');
                                readCommand(buffer,&buffDim);
                                break;
                    case CLEAR_SCREEN_CODE:

                                clear();
                                printString((uint8_t*)SHELL_MESSAGE);
                                printString(buffer);
                                break;                    
                    case '\b':
                            if (buffDim > 0)
                            {
                                removeBuffer(buffer,&buffDim);
                                putChar(c);
                            }
                            break;
                    default:
                            
                            if(addToBuffer(buffer,c,&buffDim,BUFFERSIZE)){
                                
                                putChar(c);
                            }
                            else{


                                readCommand(buffer,&buffDim);
                            }
                }
            }
        }
        return 0;
}

static void help()
{
        for (uint8_t i = 0; i < COMMANDS; i++)
        {   
            printf(" %c%s: %s\n",BULLET_POINT,commandVec[i].name,commandVec[i].description);
            
        }
}

static void readCommand(uint8_t * buffer, uint8_t* buffDim)
    {   
        uint8_t index = 0;

        if (*buffDim > 0)
        {
            splitArgs(buffer,buffDim);
            while (strcmp(commandVec[index].name, arguments[0]) != 0 && index < COMMANDS){
                index++;
            }
            if(index<COMMANDS){
                if(commandVec[index].parameters == argDim){
                    if(commandVec[index].parameters == 1){
                        commandVec[index].function();
                    }
                    else if (commandVec[index].parameters == 2)
                    {
                        commandVec[index].function(arguments[1]);
                    }
                    else if(commandVec[index].parameters == 3){
                        commandVec[index].function(arguments[1],arguments[2]);
                    }
                }
                else
                      printString((uint8_t *)"Numero erroneo de argumentos\n");
            }
            else{
                
                printString((uint8_t *)"No existe el comando\n");
            }
            cleanArgs();
        }
        printString((uint8_t*)SHELL_MESSAGE);
        cleanBuffer(buffer,buffDim);
}


static void splitArgs(uint8_t * buffer,uint8_t * buffDim){
    uint8_t j = 0,i=0;
    while(i<*buffDim && argDim < ARG_MAX){ //mientras i < a la dimension del buffer y la cantidad de argumentos sea menor a los que permito

        while (buffer[i] != 0 && buffer [i] != ' ' && j < ARG_SIZE - 1){ // j < a la cantidad de letras que permito por argumento
            arguments[argDim][j++] = buffer[i++];
        }
        arguments[argDim++][j]=0; //Pongo el 0 al final
        j=0;
        while (buffer[i] != 0 && buffer[i] == ' '){ //salteo los espacios intermedios
            i++;
        }
    }    
}

static void cleanArgs()
{
    uint8_t i = 0,j=0;
    while(i<argDim){
        while(arguments[i][j] != 0){
            arguments[i][j++]=0;
        }
        i++;
    }
    argDim = 0;
}

static void time()
{
    uint8_t totalHour[9]; 
    _time(totalHour);
    printf(" %cHora actual: %s\n",BULLET_POINT,totalHour);
}

static void printmem(uint8_t * dir)
{
      printString((uint8_t *)"Volcado de memoria de 32 bytes: \n");

      if (isHexaDir((char *)dir)) {
            uint8_t *address = (uint8_t *)stringHexaToNumber((uint8_t *)dir);
            uint8_t mem[TOREAD] = {0};
            uint8_t memStr[TOREAD * 2 + 1];
            uint8_t aux[4];
            getMem(mem, address, TOREAD);
            memToStr(mem, (uint8_t *)memStr, TOREAD);
            for (uint8_t i = 0,j=0; i < TOREAD; i++)
            {
                if (i % (TOREAD / 4) == 0)
                {
                    putChar('\n');
                }
                aux[0]=memStr[j++];
                aux[1]=memStr[j++];
                aux[2]=' ';
                aux[3]=0;
                printString(aux);
            }
            putChar('\n');
      } else {
            printString((uint8_t*)"No es una direcion hexadecimal de 64 bits\n");
      }
}

static void memToStr(uint8_t * mem, uint8_t* memStr, uint8_t bytesToConvert){
    uint8_t aux[3];
    uint8_t i,j;
    for (i = 0,j=0; i < bytesToConvert; i++)
    {
        uintToBase(mem[i],aux,16);
        if (mem[i] < 16){
            memStr[j++] = '0';
            memStr[j++] = aux[0];
        }
        else{
            memStr[j++] = aux[0];
            memStr[j++] = aux[1];
        }
    }
    memStr[j] = 0;
}

static void inforeg(){
    printString((uint8_t*)"Impresion de registros: \n");
    uint8_t number[SIZE_OF_REGISTER + 1];
    uint64_t *registerValues = _inforeg();
    for (uint8_t i = 0; i < REGISTERS; i++) {
            uintToBaseWithLength(registerValues[i], number, 16, SIZE_OF_REGISTER + 1);
            printf(" %c %s: %s\n",BULLET_POINT,registers[i],number);
    }
}


static void ps(){
    _ps();
    putChar('\n');
}


static void loop(uint8_t * seconds){
    if(atoi(seconds) < 0){
        return;
    }

    char * argv[]= {"./loopProcess",(char *)seconds};
    addNewProcess(&loopProcess,2,argv);

}

static int loopProcess(int argc, char *argv[]){
    
    int pid = getPid();
    int seconds = atoi((uint8_t*)argv[1]);
    // printf("argv[1]: %s\n", argv[1]);
    // printf("atoi: %d\n", seconds);
    while(1){
        sleep(seconds);
        printf("\nHola! %d\n",pid);
    }
    return 1;
}

static void sleep(int seconds){
    int totalTime = getSecondsElapsed()+seconds;

    while(getSecondsElapsed() <= totalTime){
        
    };
}


static void nice(uint8_t * pid,uint8_t * priority){
    _nice(atoi(pid),atoi(priority));
}

static void block(uint8_t * pid){
   _block(atoi(pid));
}

static void kill(uint8_t * pid){
    _kill(atoi(pid));
}


static void DivideByZeroException(){
    int a=1;
    int b=0;
    a=a/b;
    return;
}

static void InvalidOpcodeException(){
    _invalidOpcodeException();
    return;
}

static void information(){
    uint8_t buff1[15];
    _information(buff1);
    uint8_t mod=_model();
    uint8_t fam=mod;
    mod = mod & 0xF;
    fam = (fam >> 4) & 0xF;
    printf(" %cMarca del procesador: %s\n",BULLET_POINT,buff1);
    uintToBase(mod,buff1,10);
    printf(" %cModelo: %s\n",BULLET_POINT,buff1);
    uintToBase(fam, buff1, 10);
    printf(" %cFamily ID: %s\n",BULLET_POINT,buff1);
}

static void temperature(void){
    uint8_t str[4];
    uint8_t temp = _temperature();
    uintToBase(temp,str,10);
    printf(" %cTemperatura del CPU: %s\n",BULLET_POINT,str);
}