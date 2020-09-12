/* sampleCodeModule.c */
#include <terminal.h>
#include <calculator.h>
#include <applications.h>

int main() {
  
  addApplication(&runCalculator,CALC_MESSAGE);
  addApplication(&runTerminal,SHELL_MESSAGE);
 
  runApp();
  return 1;

}