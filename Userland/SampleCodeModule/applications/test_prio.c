#include <stdint.h>
#include <selfLib.h>
#include <stringFunctions.h>
#include <commands.h>

 #define MINOR_WAIT 1000000                               // TODO: To prevent a process from flooding the screen
 #define WAIT      10000000                              // TODO: Long enough to see theese processes beeing run at least twice

void bussy_wait(uint64_t n){
   uint64_t i;
   for (i = 0; i < n; i++);
 }

 int endless_loop2(int argc,char *argv[]){
   uint64_t pid = getPid();

   while(1){
     printf("%d ",pid);
     bussy_wait(MINOR_WAIT);
   }
   return 0;
 }

#define TOTAL_PROCESSES 3

int test_prio(int argc, char*argv[]){
  uint64_t pids[TOTAL_PROCESSES];
  uint64_t i;

   for(i = 0; i < TOTAL_PROCESSES; i++){
     char * argAux[]={"endless_loop"};
     pids[i] = addNewProcess(&endless_loop2,1,argAux);
   }

   bussy_wait(WAIT);
   printf("\nCHANGING PRIORITIES...\n");

   for(i = 0; i < TOTAL_PROCESSES; i++){
     switch (i % 3){
       case 0:
         _nice(pids[i], 0); //lowest priority 
         break;
       case 1:
         _nice(pids[i], 1); //medium priority
         break;
       case 2:
         _nice(pids[i], 2); //highest priority
         break;
     }
   }

   bussy_wait(WAIT);
   printf("\nBLOCKING...\n");

   for(i = 0; i < TOTAL_PROCESSES; i++)
     _block(pids[i]);

   printf("CHANGING PRIORITIES WHILE BLOCKED...\n");
   for(i = 0; i < TOTAL_PROCESSES; i++){
     switch (i % 3){
       case 0:
         _nice(pids[i], 1); //medium priority 
         break;
       case 1:
         _nice(pids[i], 1); //medium priority
         break;
       case 2:
         _nice(pids[i], 1); //medium priority
         break;
     }
   }

   printf("UNBLOCKING...\n");

   for(i = 0; i < TOTAL_PROCESSES; i++)
     _block(pids[i]);

   bussy_wait(WAIT);
   printf("\nKILLING...\n");

   for(i = 0; i < TOTAL_PROCESSES; i++)
     _kill(pids[i]);

  return 0;
}

