#include "defs.h"
#include "artist_manager.h"
#include "error_checking.h"
#include <sys/wait.h>
#include <sys/types.h>
#define BOOLEAN char
#define TRUE 1
#define FALSE 0

struct artist{
	pid_t pid;
	struct artist* next;
	BOOLEAN assigned;
};

struct artist* head = NULL;

void newArtist(pid_t pid){
	struct artist* new = malloc(sizeof(struct artist));
	new->pid = pid;
	new->next = NULL;
	new->assigned = FALSE;
	struct artist* cursor = head;
	struct artist* prev = NULL;
	while(cursor){
		prev = cursor;
		cursor = cursor->next;
	}
	if (prev)
		prev->next = new;
	else 
		head = new;
}

void hire(int n){
	pid_t pid;
	int i;
	for (i=0; i<n; i++){
		if ((pid = Fork()) == 0){
			artistProcess();
			exit(EXIT_FAILURE); // artistProcess() doesn't return
		}
		newArtist(pid);
	}
}

void assign(pid_t x){
	struct artist* cursor = head;
	while (cursor){
		if (cursor->pid == x){
			cursor->assigned = TRUE;
			Kill(x, SIGUSR1);
			return;
		}
		cursor = cursor->next;
	}
	fprintf(stderr, "Error: there exists no artist with PID %d.\n", x);
}

void withdraw(pid_t x){
	struct artist* cursor = head;
	while (cursor){
		if (cursor->pid == x){
			cursor->assigned = FALSE;
			Kill(x, SIGUSR2);
			return;
		}
		cursor = cursor->next;
	}
	fprintf(stderr, "Error: there exists no artist with PID %d.\n", x);
}

void fire(pid_t x){
	struct artist* cursor = head;
	struct artist* prev = NULL;
	while(cursor){
		if (cursor->pid == x){
			if (prev)
				prev->next = cursor->next;
			else
				head = cursor->next;
			if (cursor->assigned)
				Kill(cursor->pid, SIGUSR2);
			Kill(x, SIGINT);
			free(cursor);
			return;
		}
		prev = cursor;
		cursor = cursor->next;
	}
	fprintf(stderr, "Error: there exists no artist with PID %d.\n", x);
}

void fireall(){
	struct artist* cursor = head;
	struct artist* prev = NULL;
	head = NULL;
	while (cursor){
		if (cursor->assigned)
			Kill(cursor->pid, SIGUSR2);
		Kill(cursor->pid, SIGINT);
		prev = cursor;
		cursor = cursor->next;
		free(prev);
	}
}

void list(){
	char output[256];
	struct artist* cursor = head;
	while (cursor){
		sprintf(output, "%d %s\n", (int)(cursor->pid), cursor->assigned? "ASSIGNED" : "WAITING");
		cse320_print(output);
		cursor = cursor->next;
	}
}

void reapAndRemove(int sig){
	pid_t pid;
	struct artist* cursor;
	struct artist* prev;
	while((pid = waitpid(-1, NULL, WNOHANG)) > 0){
		cursor = head;
		prev = NULL;
		while(cursor){
			if (cursor->pid == pid){
				if (prev)
					prev->next = cursor->next;
				else
					head = cursor->next;
				free(cursor);
				break;
			}	
			prev = cursor;
			cursor = cursor->next;
		}
	}
}

void childFree(){
	struct artist* cursor = head;
	struct artist* temp;
	while (cursor){
		temp = cursor;
		cursor = cursor->next;
		free(temp);
	}
}
