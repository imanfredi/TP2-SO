#include <keyboardDriver.h>


static uint8_t lshift = 0, rshift = 0, blqMayus = 0, ctrl = 0;
static uint8_t action(uint8_t scanCode);
static void addBuffer(uint8_t c);
static uint8_t secondKey(uint8_t character);
static uint8_t isLetter(uint8_t character);
static uint8_t buffer[BUFFER_KEYBOARD] = {0};
static uint8_t size = 0;
static void updateRegisters(uint64_t * stackPointer); 
static uint64_t registerValues[REGISTERS]={0};
static const char pressCodes[KEYS][2] =
    {{0, 0}, {0, 0}, {'1', '!'}, {'2', '@'}, {'3', '#'},  //Es una tecla vacia y despues el esc
     {'4', '$'},{'5', '%'},{'6', '^'},{'7', '&'},{'8', '*'},
     {'9', '('},{'0', ')'},{'-', '_'},{'=', '+'},{'\b', '\b'},  //Es el backspace
     {'\t', '\t'},{'q', 'Q'},{'w', 'W'},{'e', 'E'},{'r', 'R'},  //Es el tab
     {'t', 'T'},{'y', 'Y'},{'u', 'U'},{'i', 'I'},{'o', 'O'},
     {'p', 'P'},{'[', '{'},{']', '}'},{'\n', '\n'},{0, 0},  //Es el enter y despues el control
     {'a', 'A'},{'s', 'S'},{'d', 'D'},{'f', 'F'},{'g', 'G'},
     {'h', 'H'},{'j', 'J'},{'k', 'K'},{'l', 'L'},{';', ':'},
     {'\'', '\"'},{'`', '~'},{0, 0},{'\\', '|'},{'z', 'Z'},  //L_SHIFT
     {'x', 'X'},{'c', 'C'},{'v', 'V'},{'b', 'B'},{'n', 'N'},
     {'m', 'M'},{',', '<'},{'.', '>'},{'/', '?'},{0, 0},  //RIGHT_SHIFT
     {'*', '*'},{0, 0},{' ', ' '},{0, 0}};  //ALT_IZQ Y BLOQS_MAYUS


void activateKeyBoard(uint64_t * stackPointer)
{
        uint8_t key;
        if (hasKey() == 1)
        {   
            key = getKey();
            uint8_t actionKey = action(key);
            if (actionKey == PRESSED)
            {
                switch (key)
                {
                case L_SHFT_PRESSED:
                    lshift = 1;
                    break;
                case R_SHFT_PRESSED:
                    rshift = 1;
                    break;
                case BLQ_MAYUS:
                    if (blqMayus == 1)
                    {
                        blqMayus = 0;
                    }
                    else
                        blqMayus = 1;
                    break;
                case L_CTRL_PRESSED:
                    ctrl = 1;
                    break;

            default:
                if (pressCodes[key][0] != 0)
                {
                    if(ctrl && !lshift && !rshift){
                        if(pressCodes[key][0] == 'c' || pressCodes[key][0] == 'C'){
                            addBuffer(EOF);
                        }
                        else if(pressCodes[key][0]=='l'){
                            addBuffer(CLEAR_SCREEN);
                        }
                        else if(pressCodes[key][0] == 'r'){
                            updateRegisters(stackPointer);
                        }
                    }
                    else if(secondKey(pressCodes[key][0])){
                        addBuffer(pressCodes[key][1]);
                    }
                    else{
                        addBuffer(pressCodes[key][0]);
                    }
                }
            }
        }
        else if (actionKey == RELEASED)
        {
            if (key == L_SHFT_RELEASED)
            {
                lshift = 0;
            }
            else if (key == R_SHFT_RELEASED)
            {
                rshift = 0;
            }
            else if(key == L_CTRL_RELEASED){
                ctrl = 0;
            }
        }
    }
}

static uint8_t action(uint8_t scanCode)
{
    if (scanCode >= 0x00 && scanCode <= 0x3A)
        return PRESSED;
    else if (scanCode >= 0x80 && scanCode <= 0xBA)
        return RELEASED;

    return ERRROR;
}

static uint8_t secondKey(uint8_t character)
{
    uint8_t letter = isLetter(character);
    return ( ((lshift || rshift) && (!blqMayus || !letter)) || (blqMayus && letter && !lshift && !rshift));
}

static uint8_t isLetter(uint8_t character)
{
    return (character >= 'a' && character <= 'z') || (character >= 'Z' && character <='Z');
}

static void addBuffer(uint8_t c){
    buffer[size%BUFFER_KEYBOARD] = c;
    size++;
}

uint8_t getCharacter(){
    
    uint8_t c = buffer[0];
    if(size!=0){
        memcpy(buffer,buffer+1,size);
        size--;
    }
    return c;
}

//   "RIP: ", "RSP: ", "RAX: ", "RBX: ", "RCX: ", "RDX: ", "RSI: ", "RBP: ", "RDI: ", "R8 : ", "R9 : ", "R10: ", "R11: ", "R12: ", "R13: ", "R14: ", "R15: ";
static void updateRegisters(uint64_t * stackPointer) {
    int i;
      for (i = 0; i < REGISTERS - 2; i++) {
            registerValues[REGISTERS - 1 - i] = stackPointer[i];
      }
    registerValues[0]=stackPointer[i];
    registerValues[1]=stackPointer[i+3];
}

uint64_t getRegisters(){
    return (uint64_t)registerValues;
}