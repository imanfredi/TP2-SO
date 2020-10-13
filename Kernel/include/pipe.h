#ifndef _PIPE_H
#define _PIPE_H

#define SIZE 1024
#define PIPES_MAX 30
#define NAME_MAX 25
#define NULL 0
#define STDIN_NAME "STDIN"
#define STDOUT_NAME "STDOUT"
int initPipes();
int pipeOpen(char* name);
int closePipe(int fd);
int writePipeString(char* buffer, int len, int fd);
int writePipe(int fd, char c);
char readPipe(int fd);
#endif