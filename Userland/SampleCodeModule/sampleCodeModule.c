/* sampleCodeModule.c */
#include <terminal.h>
#include <selfLib.h>

int main() {
  

 // addApplication(&runCalculator,CALC_MESSAGE);
 // addApplication(&runTerminal,SHELL_MESSAGE);
  
  char *argv[] = {"./shell"};
  addNewProcess(&runShell,1,argv);
  while(1);
  return 0;
 
  

}