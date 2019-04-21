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

BOOLEAN closing = FALSE;

BOOLEAN executeCommand(char** args){
	if (!args || !*args)
		fprintf(stderr, "ERROR: Invalid command\n");
	else if (equals(*args, "in")){
		int x, n, i;
		if (*++args && *++args){
			x = atoi(*args);
			n = atoi(*--args);
			in_spawner(n, x);
		}
		else
			fprintf(stderr, "ERROR: Invalid arguments for \"in\".\n");
	}
	else if (equals(*args, "out")){
		int x, n, i;
		if (*++args && *++args){
			x = atoi(*args);
			n = atoi(*--args);
			out_spawner(n, x);
		}
		else
			fprintf(stderr, "ERROR: Invalid arguments for \"in\".\n");
	}
	else if (equals(*args, "info")){
		museum_info();
	}
	else if (equals(*args, "start")){
		if (!closing){
			closing = TRUE;
			start_closing();
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
	museum_clean();
	pthread_exit(NULL);
}

void main(void){
	Signal(SIGINT, end);
	shell_loop(16);
	museum_clean();
	pthread_exit(NULL);
}
	

