#ifndef __PROJ2_H__
#define __PROJ2_H__

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <limits.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>

void exitfunc(int sig);
void cleanup();
int semaphore_init();
void oxygen(long TI, long TB);
void hydrogen(long TI, long TB);
int main(int argc, char* argv[]);

#endif
