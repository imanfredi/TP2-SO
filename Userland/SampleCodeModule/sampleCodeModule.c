// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* sampleCodeModule.c */
#include <selfLib.h>
#include <terminal.h>

int main() {
    char *argv[] = {"./shell"};
    addNewProcess(&runShell, 1, argv, FOREGROUND,NULL);
    return 0;
}