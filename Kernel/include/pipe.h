#ifndef _PIPE_H
#define _PIPE_H

#define SIZE 1024
#define PIPES_MAX 30
#define NAME_MAX 25
#define NULL 0
#define STDIN_NAME "STDIN"
#define STDOUT_NAME "STDOUT"
int initPipes();
int pipe(char* name);
int close(int fd);
int writePipeString(int fd, char* buffer);
int writePipe(int fd, char c);
char readPipe(int fd);
#endif