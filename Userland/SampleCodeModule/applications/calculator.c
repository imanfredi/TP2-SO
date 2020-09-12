#include <calculator.h>

static char *errorList[] = {"Division Por Cero", "Caracter invalido", "Parentesis desapareados", "Expresion invalida"};
static int state;

static void calculate(uint8_t *buffer);
static int builtExpression(calcElemT *postFij, uint8_t *buffer);
static void evaluate(calcElemT *postFij, int size, double *result);
static void operate(double op1, double op2, int operador, double *aux);
static int getPrecedence(uint8_t op1, uint8_t op2);
static int push(uint8_t *stack, uint8_t c, int *size);
static uint8_t pop(uint8_t *stack, int *size);
//Cada posicion corresponde con la tabla ascii a partir del '%' :
//                        {%, & , ',  (, ), *, +, ',' ,-}. 
static int precedence[] = {1, -1, -1, 2, 2, 1, 0, -1, 0};

void runCalculator(uint8_t * buffer,uint8_t * buffDim)
{  
    uint8_t c;
    while ((c = getChar()) != EXIT_CODE)
    {
        if (c != 0)
        {
            switch (c)
            {
            case '=':
                addToBuffer(buffer, ' ',buffDim);
                putChar('=');
                calculate(buffer);
                printString((uint8_t*)CALC_MESSAGE);

                cleanBuffer(buffer,buffDim);
                break;
            case CLEAR_SCREEN_CODE:
                clear();
                printString((uint8_t*)CALC_MESSAGE);
                printString(buffer);
                break;
            case '\b':
            
                if (*buffDim > 0)
                {
                    removeBuffer(buffer, buffDim);
                    putChar('\b');
                }
                break;
            
            case '\n':
                  while (*buffDim > 0) {
                        putChar('\b');
                        removeBuffer(buffer, buffDim);
                  }
                  break;
            default:
                    if(addToBuffer(buffer, c, buffDim)){
                        putChar(c);
                    }else{
                        calculate(buffer);
                        cleanBuffer(buffer,buffDim);

                    }
            }
        }
    }

}

static void calculate(uint8_t *buffer)
{
    calcElemT postFija[BUFFER_MAX] = {{0,0}};
    state = OK;
    int size = builtExpression(postFija, buffer);
    
 
    if (state != OK)
    {   
        printf("\nERROR: %s\n",errorList[state]);

    }
    else
    {
        double result = 0;
        evaluate(postFija, size,&result);
        if (state != OK)
        {   
            printf("\nERROR: %s\n",errorList[state]);
        }
        else
        {   uint8_t s[NUMBER_MAX];
            ftoa(result,s,PRECISION);
            printf("\nResultado: %s\n",s);
        }
    }
}

static int builtExpression(calcElemT *postFij, uint8_t *buffer)
{
    
    //RECOLECTORES:
    uint8_t stack[BUFFERSIZE] = {0};
    
    //AUXILIARES:
    uint8_t c;
    double num;
    int to_Num;
    double saveNum = 0;

    //ITERADORES Y DIMENSIONES;
    int dimStack = 0;
    int dimPostFija = 0;
    int i = 0; //Para recorrer el buffer 

    //FLAGS:
    int punto = 0;
    int number = 0;
    int espacio = TRUE;

    //CONTADORES:
    int menos = 0;
    int parentesis = 0;
    int cantDec = 0;

    while ((c = buffer[i++]) != 0)
    {     

        /*Aca termino de escribir el numero*/
        if(espacio==FALSE && c!=' ' && !isDigitOrPoint(c)){
            state=INV_EXP;
            return -1;
        }

        switch (c){  
                    
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '0':

                to_Num = toNumber(c);//Me guardo el numero
                    
                if(punto){   
                    mathPow(10,--cantDec,&num);
                    saveNum+=num*to_Num;
                }
                else{
                    saveNum= saveNum*10+to_Num;
                }
                number=TRUE;
                espacio=FALSE;

                break;
            case '.':
                espacio=FALSE;
                punto = TRUE;
                break;             
            case ' ':

                if (number){

                    if(menos%2!=0){
                        saveNum*=-1;
                    }
                    postFij[dimPostFija].element=saveNum;
                    postFij[dimPostFija++].type=NUMBER;

                    number=FALSE;
                    punto=FALSE;
                    saveNum=0;
                    cantDec=0;
                }
                else if (menos){

                    while( dimStack > 0 && getPrecedence('-',stack[dimStack-1]) ){
                            postFij[dimPostFija].element = pop(stack,&dimStack);
                            postFij[dimPostFija++].type = OPERATOR;
                    }
                
                    push(stack,'-',&dimStack);
                    
                }

                menos=0;
                espacio=TRUE;
                break;

            case '-':
                menos++;
                break;
            case '(':
                parentesis++;            
            case '+':
            case '%':
            case '*':
                espacio=FALSE;

                   while( dimStack > 0 && getPrecedence(c,stack[dimStack-1]) ){
                            postFij[dimPostFija].element = pop(stack,&dimStack);
                            postFij[dimPostFija++].type = OPERATOR;
                    }
    
                    push(stack,c,&dimStack);
                    
                    break;

            case ')':
                espacio=FALSE;
                parentesis--;
                if (parentesis < 0)
                {
                    state = BRACKET_ERROR;
                    return -1;
                }
                while (dimStack > 0 && stack[dimStack - 1] != '(')
                {
                    postFij[dimPostFija].element = pop(stack,&dimStack);
                    postFij[dimPostFija++].type = OPERATOR;
                }
                pop(stack,&dimStack);
                break;

            default:
                state = INVALID_CHAR;
                return -1;
            }
    }

    if (parentesis != 0)
    {
        state = BRACKET_ERROR;
        return -1;
    }
    
    while (dimStack > 0)
    {

        postFij[dimPostFija].element = pop(stack,&dimStack);
        postFij[dimPostFija++].type = OPERATOR;
    }
    return dimPostFija;
}

static void evaluate(calcElemT *postFij, int size,double * result)
{

    int i = 0;
    int auxDim = 0;
    double aux[20] = {0};
    while (i < size && state==OK)
    {

        if (postFij[i].type == NUMBER)
        {
            aux[auxDim] = postFij[i].element; 
        }
        else{ 
            
            if (auxDim < 2){
                state = INV_EXP;
                return;
            }

            operate(aux[auxDim-2], aux[auxDim - 1],(int)postFij[i].element, aux+auxDim-2); 
            auxDim -= 2;
        }
        auxDim++;
        i++;
    }
    
    if(state!=OK){
        return;
    }
    if (auxDim != 1){
        state = INV_EXP;
        return;
    }

    *result = aux[0];

}

static void operate(double op1, double op2, int operador,double * aux)
{
    double abs_aux;
    switch ((uint8_t)operador)
    {
        case '+':
            (*aux) = op1 + op2;
            break;
        case '-':
            (*aux) = op1 - op2;
            break;
        case '%':
            Abs(op2, &abs_aux);
            if (abs_aux < EPSILON)
               state = DIV_BY_ZERO;
            else
                (*aux) = op1 / op2;
            break;
        case '*':
            (*aux) = op1 * op2;
            break;
        default:
            state = INV_EXP;

    }

}

static int getPrecedence(uint8_t op1,uint8_t op2){

    if(op2=='('){
        return 0;
    }
    return precedence[op1 % FIRST]<=precedence[op2 % FIRST];
}

static int push(uint8_t *stack, uint8_t c, int *size)
{

    if (*size == BUFFERSIZE - 1)
    {
        return ERROR;
    }

    stack[*size] = c;
    (*size)++;
    
    return OK;
}

static uint8_t pop(uint8_t *stack, int *size)
{
    if (*size == 0)
    {
        return ERROR;
    }

    uint8_t c = stack[(*size) - 1];
    (*size)--;
    return c;
}
