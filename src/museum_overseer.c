#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include "museum_overseer.h"
#include "error_checking.h"
#include "shell.h"
#define equals(s1, s2) !strcmp(s1, s2)
#define BOOLEAN char
#define TRUE 1
#define FALSE 0

extern BOOLEAN closing;

BOOLEAN executeCommand(char** args){
	if (!args || !*args)
		fprintf(stderr, "ERROR: Invalid command\n");
	else if (equals(*args, "in")){
		int x, n, i;
		if (*++args && (n = atoi(*args)) <= 5 && n >= 1 && *++args && (x = atoi(*args)) > 0){
			in_spawner(n, x);
		}
		else
			fprintf(stderr, "ERROR: Invalid arguments for \"in\".\n");
	}
	else if (equals(*args, "out")){
		int x, n, i;
		if (*++args && (n = atoi(*args)) <= 5 && n >= 1 && *++args && (x = atoi(*args)) > 0){
			out_spawner(n, x);
		}
		else
			fprintf(stderr, "ERROR: Invalid arguments for \"out\".\n");
	}
	else if (equals(*args, "info")){
		museum_info();
	}
	else if (equals(*args, "start")){
		if (!closing){
			closing = TRUE;
			start();
		}
	}
	else if (equals(*args, "exit")){
		return FALSE;
	}
	else
		fprintf(stderr, "ERROR: Invalid command\n");
	return TRUE;
}

void end(int sig){
	closing = FALSE;
	museum_clean();
	pthread_exit(NULL);
}

void main(void){
	Signal(SIGINT, end);
	closing = FALSE;
	initialize_museum_ds();
	shell_loop(16);
	closing = FALSE;
	museum_clean();
	pthread_exit(NULL);
}
	

