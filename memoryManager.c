/*Implementacion basada en el libro de K&R*/

#define NULL (char *)0
#define HEAP_SIZE 1024 * 1024 * 2  //la maxima cantidad que soporta nuestro memory manager

#include <stdio.h>
#include <string.h>

typedef long Align;

union header { /* header del bloque */
    struct {
        union header *next; /* proximo header de un bloque */
        unsigned size;      /* tamaño del bloque  */
    } s;
    Align x; /* fuerza el alineamiento de bloques*/
};

typedef union header Header;

static char heap[HEAP_SIZE];  //este de aca vuela despues
static Header *freep;
static char *heapBase;
static unsigned long heapSize;

void *malloc2(unsigned nbytes);
void free2(void *ptr);
void initialize(char *heapInit, unsigned long size);

int main(int argc, char const *argv[]) {
    initialize(heap, HEAP_SIZE);
    int *ptr1 = malloc2(524272);
    int *ptr2 = malloc2(524272);
    int *ptr3 = malloc2(524272);
    int *ptr4 = malloc2(524272);

    free2(ptr1);
    free2(ptr2);
    free2(ptr3);
    free2(ptr4);
  
    return 0;
}


void *malloc2(unsigned nbytes) {
    Header *curr, *prevp;
    unsigned nunits;
   
    if(nbytes == 0)
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

void initialize(char *heapInit, unsigned long size) {
    heapBase = heapInit;

    if (size > HEAP_SIZE)
        heapSize = HEAP_SIZE;
    else
        heapSize = size;

    freep = (Header *)heapBase;
    freep->s.size = heapSize / sizeof(Header);
    freep->s.next = NULL;
}

void free2(void *ptr) {
    Header *curr, *prev, *aux;
    aux = (Header *)(ptr);
    aux--;

    if(aux == NULL || aux<(Header*)heapBase || aux>= (Header*)heapBase+heapSize){
        return;
    }

    /*Fijarse si el malloc esta entre los valores del heap y si es null ver que hacer*/

    curr = freep;
        while (curr != NULL && curr < aux) {/* recorria nodos hasta encontrar el anterior libre*/
            prev = curr;
            curr = curr->s.next;
        }
        if(curr != NULL && (aux + aux->s.size == curr)){ /*Con esto veo si me puedo unir con el de adelante*/
            aux->s.size += curr-> s.size;
            aux->s.next = curr->s.next; 
        }
        else                                /*Si no me puedo unir hago simplemente un cambio de puntero*/
            aux->s.next = curr;
        
        if(curr == freep){ /*Con esto veo si estoy antes que el primero libre*/
            freep = aux;
        }
        else{
            if (prev + prev->s.size == aux) { /*Uno los bloques si tengo uno libre atrás*/
                prev->s.size += aux->s.size;
                prev->s.next = aux->s.next;
            }
            else {
                prev->s.next = aux; /*Si no hago que el de atrás me apunte*/
            }
        }
        
        
}

/*REVISAR ESTA

void free3(void *ptr) {
    Header *curr, *prev, *aux;
    aux = (Header *)(ptr - 1);
                                //Fijarse si el malloc esta entre los valores del heap y si es null ver que hacer
  
    curr = freep;
    if (curr < aux) {
        while (curr != NULL && curr < aux) {
            prev = curr;
            curr = curr->s.next;
        }
        if (prev + prev->s.size == aux) { //Uno los bloques si estan juntos
            prev->s.size += aux->s.size;
            prev->s.next = curr;
        } else {
            aux->s.next = curr;
            prev->s.next = aux;
        }
    } else {
        if (aux + aux->s.size == curr) {
            aux->s.size += curr->s.size;
            aux->s.next = curr->s.next;
        } else {
            aux->s.next = curr;
        }
        freep = aux;
    }
}
*/