#include <utilsKernel.h>

int isOperator(char character){
    if(character == '+' || character == '*' || character == '%'|| character=='^'||character=='-'){
        return 1;
    }
    return 0;
}

int isDigit(char c){
    return c >= '0' && c <= '9'; 
}

int toNumber(char character){

    if(isDigit(character))
        return character - '0';

    return -1;
}

uint64_t pow(uint64_t base, uint64_t exp)
{
    uint64_t res = 1;
    for (uint64_t i = 0; i < exp; i++)
    {
        res *= base;
    }
    return res;
}

int isDigitOrPoint(char c)
{
    return isDigit(c) || c == '.';
}

void mathPow(int base, int exp,double * number)
{
    *number = 1;
    if(exp >=0){
        for (int i = 0; i < exp; i++)
        {
            (*number)*=base;
        }
    }
    else{
        for (int i = 0; i >exp; i--)
        {
            (*number)/=base;
        }   
    }

}

void Abs(double number, double * aux){
    *aux = number;
    if(number <0){
        (*aux)*= -1;
    }
}

int isHexaDir(char * dir){
    int i;
    for (i = 0; dir[i] != 0 && i<HEXADIRSIZE; i++){
        if( !(isDigit(dir[i]) || isHexaLetter(dir[i])) ){
            return 0;
        }
    }
    return i<HEXADIRSIZE && dir[i] == 0?1:0;    
}

int isHexaLetter(char c){
    return (c>='a' && c<='f') || (c>='A' && c<='F');
}