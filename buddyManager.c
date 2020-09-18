#include <stdint.h>

#define HEAP_SIZE 1024 * 1024 * 10
#define MAX_ALLOC_LOG_2 21  //1024*1024*2 --> 2^21
#define MIN_ALLOC_LOG_2 6   // 2^6
#define BUCKET_COUNT_MAX (MAX_ALLOC_LOG_2 * MIN_ALLOC_LOG_2 +1)
#define MAX_ALLOC ((size_t)1 << MAX_ALLOC_LOG2)
#define MIN_ALLOC ((size_t)1 << MIN_ALLOC_LOG2)

#define TRUE 1
#define FALSE 0

typedef struct {
    list* next;
    list* prev;
    char isFree;
    char isSplit;
} list_t;

typedef list_t* list;

list_t buckets[BUCKET_COUNT_MAX];  //la cantidad de niveles que tengo. Cada lista indica los bloques de ese tamaño que se encuentran disponibles en ese nivel
static char heap[HEAP_SIZE];
static unsigned bucketSize;
static unsigned long heapSize;
static int bucketCount;
static list_t* heapBase;

void * malloc2(unsigned nbytes);
void free2(void* ptr);
void initialize(void* heapInit, unsigned long size);

int main(int argc, char const* argv[]) {
    initialize(heap, HEAP_SIZE);
    return 0;
}

void * malloc2(unsigned nbytes){
    if(nbytes + sizeof(list_t) > MAX_ALLOC)
        return NULL;

    int minBucket = bucket_for_request(nbytes + sizeof(list_t));

    for (int i = 0; i < minBucket; i++) {
        
    }
    
    //Buscar el bucket mas chico donde entraria el tamaño solicitado
    //no se fija si hay espacio o esta ocupado

    //Busca un bucket con una lista llena del mismo tamaño o mas grande
    // que lo que necesitamos. Si no es exacto vamo a splitear hasta tener un match

}

void free2(void* ptr) {
}

void initialize(void* heapInit, unsigned long size) {
    
    heapSize = size;
    heap_base = heapInit;
    bucket_count = 
    
    initList(&buckets[0], heap_base);

}

unsigned long getBucketSize(int bucket) {
}

/*
    h5---h8---h6-------h4---------------h2----------------------------------
    h
*/
void initList(list ptr, list dir) {
    ptr->next = dir;
    ptr->prev = dir;
    ptr->isFree = TRUE;
    ptr->isSplit = FALSE;
}

void pushList() {
}

/*
    5 bytes --> 2^21 

    5+sizeof(list_t) ;


    bucket_for_request()


*/

static size_t bucket_for_request(size_t bytes) {
    size_t bucket = BUCKET_COUNT - 1;
    size_t size = MIN_ALLOC;

    while (size < bytes) {
        bucket--;
        size *= 2;
    }

    return bucket;
}