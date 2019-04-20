#include "defs.h"
#include "artist_manager.h"
#include "error_checking.h"
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>
#define BOOLEAN char
#define TRUE 1
#define FALSE 0

struct artist{
	pthread_t tid;
	struct artist* next;
	sem_t mutex;
	BOOLEAN assigned;
	BOOLEAN alive;
};

struct artist* head = NULL;

struct artist* newArtist(pthread_t tid){
	struct artist* new = malloc(sizeof(struct artist));
	new->tid = tid;
	new->next = NULL;
	new->assigned = FALSE;
	new->alive = TRUE;
	sem_init(&(new->mutex), 0, 1);

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
	return new;
}

void hire(int n){
	pthread_t tid;
	int i;
	for (i=0; i<n; i++){
		struct artist* ptr = newArtist(tid);
		pthread_create(&tid, NULL, artistProcess, (void *)ptr);
	}
}

void assign(pthread_t x){
	struct artist* cursor = head;
	while (cursor){
		if (cursor->tid == x){
			sem_wait(&(cursor->mutex));
			cursor->assigned = TRUE;
			sem_post(&(cursor->mutex));
			return;
		}
		cursor = cursor->next;
	}
	fprintf(stderr, "Error: there exists no artist with TID %d.\n", (int)x);
}

void withdraw(pthread_t x){
	struct artist* cursor = head;
	while (cursor){
		if (cursor->tid == x){
			sem_wait(&(cursor->mutex));
			cursor->assigned = FALSE;
			sem_post(&(cursor->mutex));
			return;
		}
		cursor = cursor->next;
	}
	fprintf(stderr, "Error: there exists no artist with TID %d.\n", (int)x);
}

void fire(pthread_t x){
	struct artist* cursor = head;
	struct artist* prev = NULL;
	while(cursor){
		if (cursor->tid == x){
			if (prev)
				prev->next = cursor->next;
			else
				head = cursor->next;
			sem_wait(&(cursor->mutex));
			if (cursor->assigned)
				cursor->assigned = FALSE;
			cursor->alive = FALSE;
			sem_post(&(cursor->mutex));
			return;
		}
		prev = cursor;
		cursor = cursor->next;
	}
	fprintf(stderr, "Error: there exists no artist with TID %d.\n", (int)x);
}

void fireall(){
	struct artist* cursor = head;
	head = NULL;
	while (cursor){
		sem_wait(&(cursor->mutex));
		if (cursor->assigned)
			cursor->assigned = FALSE;
		cursor->alive = FALSE;
		cursor = cursor->next;
		sem_post(&(cursor->mutex));
	}
}

void list(){
	char output[256];
	struct artist* cursor = head;
	while (cursor){
		sem_wait(&(cursor->mutex));
		sprintf(output, "%d %s\n", (int)(cursor->tid), cursor->assigned? "ASSIGNED" : "WAITING");
		cse320_print(output);
		cursor = cursor->next;
		sem_post(&(cursor->mutex));
	}
}
