#include "error_checking.h"
#include "defs.h"
#include "artist_manager.h"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#define BOOLEAN char
#define TRUE 1
#define FALSE 0

BOOLEAN check(struct artist* this, BOOLEAN assigned){
	if (assigned != this->assigned){
		char msg[256];
		if (this->assigned)
			sprintf(msg, "ARTIST %d IS ASSIGNED TO A JOB\n", (int)this->tid);
		else
			sprintf(msg, "ARTIST %d IS WITHDRAWN FROM A JOB\n", (int)this->tid);
		cse320_print(msg);
		assigned = this->assigned;
	}
	return assigned;
}

void* artistProcess(void* voidptr){
	pthread_detach( pthread_self() );

	struct artist* this = (struct artist*)voidptr;
	BOOLEAN assigned = this->assigned;
	char msg[64];

	while(this->alive){
		sem_wait(&(this->mutex));
		assigned = check(this, assigned);
		sem_post(&(this->mutex));
	}
	sem_wait(&(this->mutex));
	check(this, assigned);
	free(this);

	pthread_exit(NULL);
}
