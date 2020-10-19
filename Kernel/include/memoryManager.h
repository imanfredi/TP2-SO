#define NULL 0
void *malloc2(unsigned nbytes);
void free2(void *ptr);
void initializeMemoryManager(char *heapInit, unsigned long size);
int memInfo();