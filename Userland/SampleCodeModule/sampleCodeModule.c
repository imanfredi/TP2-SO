/* sampleCodeModule.c */
#include <selfLib.h>
#include <terminal.h>

int main() {
    char *argv[] = {"./shell"};
    addNewProcess(&runShell, 1, argv,FOREGROUND);

    return 0;
}