#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H


void * malloc2(unsigned nbytes);
void free2(void *ptr);
void initializeMemoryManager(char *heapInit, unsigned long size);

#endif