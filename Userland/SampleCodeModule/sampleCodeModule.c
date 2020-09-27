/* sampleCodeModule.c */
#include <selfLib.h>
#include <terminal.h>

int main() {
    char *argv[] = {"./shell"};
    addNewProcess(&runShell, 1, argv);

    return 0;
}