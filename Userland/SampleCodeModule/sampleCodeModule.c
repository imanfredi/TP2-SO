/* sampleCodeModule.c */
#include <terminal.h>
#include <calculator.h>
#include <applications.h>
#include <selfLib.h>

int main() {
  

 // addApplication(&runCalculator,CALC_MESSAGE);
 // addApplication(&runTerminal,SHELL_MESSAGE);
  char *argv[] = {"shell"};
  addNewProcess(&runTerminal,1,argv);
  exit(0);
  //runApp();
  

}