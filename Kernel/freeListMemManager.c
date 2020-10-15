/*Implementacion basada en el libro de K&R*/
#ifndef BUDDY_M
#include <memoryManager.h>
#include <stringFunctionsKernel.h>
#include <adminScreen.h>

#define NULL 0
#define HEAP_SIZE 1024 * 1024 * 128  //la maxima cantidad que soporta nuestro memory manager

typedef long Align;

union header { /* header del bloque */
    struct {
        union header *next; /* proximo header de un bloque */
        unsigned size;      /* tamaño del bloque  */
    } s;
    Align x; /* fuerza el alineamiento de bloques*/
};

typedef union header Header;

static Header *freep;
static char *heapBase;
static unsigned long heapSize;

void initializeMemoryManager(char *heapInit, unsigned long size) {
    heapBase = heapInit;

    if (size > HEAP_SIZE)
        heapSize = HEAP_SIZE;
    else
        heapSize = size;

    freep = (Header *)heapBase;
    freep->s.size = heapSize / sizeof(Header);
    freep->s.next = NULL;
}

void *malloc2(unsigned nbytes) {
    Header *curr, *prevp;
    unsigned nunits;

    if (nbytes == 0)
        return (void *)NULL;

    nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1;

    for (curr = freep; curr != NULL; prevp = curr, curr = curr->s.next) {
        if (curr->s.size >= nunits) {
            if (curr->s.size == nunits) {
                if (curr == freep)  //me quedo con la base de los vacíos. Esto pasa si es el primero de los free
                    freep = curr->s.next;
                else
                    prevp->s.next = curr->s.next;  //el siguiente del anterior es mi siguiente;

            } else {
                curr->s.size -= nunits;  //le saco una cantidad al header en el que estoy parado
                curr += curr->s.size;    //me corro
                curr->s.size = nunits;   // le pongo la cantidad de bloques que necesito en el que me pidieron
            }
            return (void *)(curr + 1);
        }
    }
    return (void *)NULL;
}

void free2(void *ptr) {
    Header *curr, *prev, *aux;
    aux = (Header *)(ptr);
    aux--;

    if (aux == NULL || aux < (Header *)heapBase || aux >= (Header *)heapBase + heapSize) {
        return;
    }

    curr = freep;
    while (curr != NULL && curr < aux) { /* recorria nodos hasta encontrar el anterior libre*/
        prev = curr;
        curr = curr->s.next;
    }
    if (curr != NULL && (aux + aux->s.size == curr)) { /*Con esto veo si me puedo unir con el de adelante*/
        aux->s.size += curr->s.size;
        aux->s.next = curr->s.next;
    } else /*Si no me puedo unir hago simplemente un cambio de puntero*/
        aux->s.next = curr;

    if (curr == freep) { /*Con esto veo si estoy antes que el primero libre*/
        freep = aux;
    } else {
        if (prev + prev->s.size == aux) { /*Uno los bloques si tengo uno libre atrás*/
            prev->s.size += aux->s.size;
            prev->s.next = aux->s.next;
        } else {
            prev->s.next = aux; /*Si no hago que el de atrás me apunte*/
        }
    }
}

int memInfo(){
    int i;
    Header * curr;
    int len;
    uint8_t number[40];
    char * message1="Bloque: ";
    char * message2="Cantidad de Bytes: ";
    for (i=1, curr = freep; curr != NULL ; curr=curr->s.next,i++)
    {  
        printStringScreen((uint8_t*)message1,strlen((uint8_t*)message1),BLACK_WHITE);
        
        len = uintToBase(i,number,10);
        printStringScreen(number,len,BLACK_WHITE);
        printStringScreen((uint8_t*)"  ",strlen((uint8_t*)"  "),BLACK_WHITE);
        
        printStringScreen((uint8_t*)message2,strlen((uint8_t*)message2),BLACK_WHITE);

        len = uintToBase(curr->s.size,number,10);
        printStringScreen(number,len,BLACK_WHITE);

        newLineScreen();
        
    }
    
    return 0;

}

#endif