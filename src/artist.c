#include "error_checking.h"
#include "defs.h"
#include "artist_manager.h"
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#define BOOLEAN char
#define TRUE 1
#define FALSE 0

pid_t pid;
BOOLEAN assigned;
BOOLEAN alive;

void setAssigned(int sig){
	if (!assigned){
		assigned = TRUE;
		char msg[64];
		sprintf(msg, "ARTIST %d IS ASSIGNED TO A JOB\n", (int)pid);
		cse320_print(msg);
	}
	else
		fprintf(stderr, "Error: %d is already assigned\n", (int)pid);
}

void setWaiting(int sig){
	if (assigned){
		assigned = FALSE;
		char msg[64];
		sprintf(msg, "ARTIST %d IS WITHDRAWN FROM A JOB\n", (int)pid);
		cse320_print(msg);
	}
	else
		fprintf(stderr, "Error: %d isn't assigned to a job\n", (int)pid);
}

void artistProcess(){
	childFree();

	pid = getpid();
	assigned = FALSE;
	alive = TRUE;

	Signal(SIGUSR1, setAssigned);
	Signal(SIGUSR2, setWaiting);
	Signal(SIGINT, SIG_DFL);

	while(alive)
		pause();
	
	exit(0);
}
