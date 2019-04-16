#include "defs.h"
#include "artist_manager.h"
#include "error_checking.h"
#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#define equals(s1, s2) !strcmp(s1, s2)
#define BOOLEAN char
#define TRUE 1
#define FALSE 0

char* helpstring = "help\t\tprints this very message!\n\
date\t\tprints the current date\n\
hire N\t\thires N many artists (creates processes for each)\n\
fire X\t\tfires the given artist X (kills process with PID X)\n\
fireall\t\tfires all artists (kills all child processes)\n\
assign X\tmarks the given artist X as assigned to a job (and the child process X prints it is assigned)\n\
withdraw X\tmarks the given artist X as unassigned (and the child process X prints it is withdrawn)\n\
list\t\tprints all artists in the form \"PID STATUS\" (ASSIGNED or WAITING)\n\
exit\t\texits the shell\n";

BOOLEAN executeCommand(char** args){
	if (!args || !*args)
		return TRUE;
	if (equals(*args, "help")){
		cse320_print( helpstring );
	}
	if (equals(*args, "date")){
		if (Fork() == 0){
			*args = "/bin/date";
			execvp(*args, args);
			fprintf(stderr, "Error running bin/date.\n");
			exit(EXIT_FAILURE);
		}
		pause();
	}
	if (equals(*args, "hire")){
		int n;
		if (*++args && (n = atoi(*args)) > 0)
			hire(n);
		else
			fprintf(stderr, "Error: invalid arguments for \"hire\".\n");
	}
	if (equals(*args, "fire")){
		int x;
		if (*++args && (x = atoi(*args)) > 0){
			fire((pid_t)x);
		}
		else
			fprintf(stderr, "Error: invalid arguments for \"fire\".\n");
	}
	if (equals(*args, "fireall")){
		fireall();
	}
	if (equals(*args, "assign")){
		if (*++args){
			pid_t x = (pid_t)atoi(*args);
			assign(x);
		}
		else
			fprintf(stderr, "Error: invalid arguments for \"assign\".\n");
	}
	if (equals(*args, "withdraw")){
		if (*++args){
			pid_t x = (pid_t)atoi(*args);
			withdraw(x);
		}
		else
			fprintf(stderr, "Error: invalid arguments for \"withdraw\".\n");
	}
	if (equals(*args, "list")){
		list();
	}
	if (equals(*args, "exit")){
		return FALSE;
	}
	return TRUE;
}

void end(int sig){
	fireall();
	exit(0);
}

int main(void){
	Signal(SIGINT, end);
	Signal(SIGCHLD, reapAndRemove);
	shell_loop(32);
	fireall();
	exit(0);
}
