#include <exceptions.h>

static void zero_division();
static void invalidOperationCode();
static void printRegisters();
static registerT registers = {{"RIP: ", "RSP: ", "RAX: ", "RBX: ", "RCX: ", "RDX: ", "RBP: ", "RDI: ",
                               "RSI: ", "R8 : ", "R9 : ", "R10: ",
                               "R11: ", "R12: ", "R13: ", "R14: ", "R15: "},
                              {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
static void *initialIP;
static void *initialSP;

void exceptionDispatcher(int exception) {
    switch (exception) {
        case ZERO_EXCEPTION_ID:
            zero_division();
            break;
        case INVALID_OPERATION_CODE:
            invalidOperationCode();
            break;
    }
    printRegisters();
    uint8_t *a = (uint8_t *)"PRESIONE ENTER PARA REINICIAR";
    printStringScreen(a, strlen(a), BLACK_WHITE);
}

static void zero_division() {
    uint8_t *a = (uint8_t *)"DIVISION BY CERO EXCEPTION";
    printStringScreen(a, strlen(a), BLACK_RED);
    newLineScreen();
}

static void invalidOperationCode() {
    uint8_t *a = (uint8_t *)"INVALID OPERATION CODE EXCEPTION";
    printStringScreen(a, strlen(a), BLACK_RED);
    newLineScreen();
}

void printRegisters() {
    newLineScreen();
    uint8_t *value[SIZE_OF_REGISTER + 1];
    for (int i = 0; i < REG; i++) {
        printStringScreen((uint8_t *)" ", 1, BLACK_WHITE);
        printStringScreen((uint8_t *)registers.name[i], REG_NAME - 1, BLACK_WHITE);
        uintToBaseWithLength(registers.values[i], (uint8_t *)value, 16, SIZE_OF_REGISTER + 1);
        printStringScreen((uint8_t *)value, SIZE_OF_REGISTER, BLACK_WHITE);
        newLineScreen();
    }
    newLineScreen();
}

void initExceptions(void *instructionPointer, void *stackPointer) {
    initialIP = instructionPointer;
    initialSP = stackPointer;
}

void saveRegisters(uint64_t *stackPointer) {
    int i;
    for (i = 0; i < REG - 2; i++) {
        registers.values[REG - 1 - i] = stackPointer[i];
    }
    registers.values[0] = stackPointer[i];      //guardo rip
    registers.values[1] = stackPointer[i + 3];  //guardo rsp
    stackPointer[i] = (uint64_t)initialIP;
    stackPointer[i + 3] = (uint64_t)initialSP;
}