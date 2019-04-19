#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "defs.h"

int fib(int n){
	if (n < 0){
		fprintf(stderr, "ERROR: Fibonacci number of %d is undefined!\n", n);
		exit(EXIT_FAILURE);
	}
	if (n == 0)
		return 0;
	if (n == 1)
		return 1;
	return fib(n-1) + fib(n-2);
}

int input;

void* printFib(){
	int output = fib(input);
	char buf[256];
	sprintf(buf, "%d\n", output);
	cse320_print(buf);
	return NULL;
}

void main(int argc, char** argv){
	if (argc != 2){
		fprintf(stderr, "ERROR: Invalid Arguments\n");
		exit(EXIT_FAILURE);
	}
	input = atoi(*++argv);
	pthread_t tid;
	pthread_create(&tid, NULL, printFib, NULL);
	pthread_join(tid, NULL);
	exit(0);
}

