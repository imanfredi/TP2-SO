#include <test_util.h>
#include <commands.h>
#include <selfLib.h>
#include <stringFunctions.h>

int endless_loop(int argc, char*argv[]){
   while(1);
}

#define MAX_PROCESSES 10 //Should be around 80% of the the processes handled by the kernel

enum State {RUNNING=0, BLOCKED, KILLED, ERROR};

typedef struct P_rq{
  uint32_t pid;
  enum State state;
}p_rq;

int test_processes(int argc, char*argv[]){
  p_rq p_rqs[MAX_PROCESSES];
  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;

  while (1){
     // Create MAX_PROCESSES processes
    for(rq = 0; rq < MAX_PROCESSES; rq++){
      char * argAux[] =  {"endless_loop"}; 
       p_rqs[rq].pid = addNewProcess(&endless_loop,1,argAux,BACKGROUND);  // TODO: Port this call as required
       if (p_rqs[rq].pid == -1){                           // TODO: Port this as required
         printf("Error creating process\n");               // TODO: Port this as required
         return 1;
       }else{
         p_rqs[rq].state = RUNNING;
         alive++;
       }
     }

     // Randomly kills, blocks or unblocks processes until every one has been killed
     while (alive > 0){

       for(rq = 0; rq < MAX_PROCESSES; rq++){
         action = GetUniform(2) % 2; 
          
         switch(action){
           case 0:
             if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED){
               if (_kill(p_rqs[rq].pid) == -1){          // TODO: Port this as required
                 printf("Error killing process %d\n",p_rqs[rq].pid);        // TODO: Port this as required
                 return 1;
               }
               p_rqs[rq].state = KILLED; 
               alive--;
             }
             break;

           case 1:
             if (p_rqs[rq].state == RUNNING){
               if(_block(p_rqs[rq].pid) == -1){          // TODO: Port this as required
                 printf("Error blocking process %d \n",p_rqs[rq].pid);       // TODO: Port this as required
                 return 1;
               }
               p_rqs[rq].state = BLOCKED; 
             }
             break;
         }
       }

       // Randomly unblocks processes
       for(rq = 0; rq < MAX_PROCESSES; rq++)
         if (p_rqs[rq].state == BLOCKED && GetUniform(2) % 2){
           if(_block(p_rqs[rq].pid) == -1){            // TODO: Port this as required
             printf("Error unblocking process %d \n",p_rqs[rq].pid);         // TODO: Port this as required
             return 1;
           }
           p_rqs[rq].state = RUNNING; 
         }
     } 
   }
  return 0;
}

