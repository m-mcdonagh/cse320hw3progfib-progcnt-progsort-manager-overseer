#include "defs.h"
#include "artist_manager.h"
#include "error_checking.h"
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>
#define BOOLEAN char
#define TRUE 1
#define FALSE 0

struct artist* head = NULL;

struct artist* newArtist(){
	struct artist* new = malloc(sizeof(struct artist));
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
	int i;
	for (i=0; i<n; i++){
		struct artist* ptr = newArtist();
		pthread_create(&(ptr->tid), NULL, artistProcess, (void *)ptr);
	}
}

void assign(int x){
	struct artist* cursor = head;
	while (cursor){
		if ((int)cursor->tid == x){
			sem_wait(&(cursor->mutex));
			cursor->assigned = TRUE;
			sem_post(&(cursor->mutex));
			return;
		}
		cursor = cursor->next;
	}
	fprintf(stderr, "Error: there exists no artist with TID %d.\n", (int)x);
}

void withdraw(int x){
	struct artist* cursor = head;
	while (cursor){
		if ((int)cursor->tid == x){
			sem_wait(&(cursor->mutex));
			cursor->assigned = FALSE;
			sem_post(&(cursor->mutex));
			return;
		}
		cursor = cursor->next;
	}
	fprintf(stderr, "Error: there exists no artist with TID %d.\n", (int)x);
}

void fire(int x){
	struct artist* cursor = head;
	struct artist* prev = NULL;
	while(cursor){
		if ((int)cursor->tid == x){
			if (prev)
				prev->next = cursor->next;
			else
				head = cursor->next;
			sem_wait(&(cursor->mutex));
			if (cursor->assigned)
				cursor->assigned = FALSE;
			cursor->alive = FALSE;
			sem_post(&(cursor->mutex));
			pthread_join(cursor->tid, NULL);
			free(cursor);
			return;
		}
		prev = cursor;
		cursor = cursor->next;
	}
	fprintf(stderr, "Error: there exists no artist with TID %d.\n", (int)x);
}

void fireall(){
	struct artist* cursor = head;
	struct artist* temp;
	head = NULL;
	while (cursor){
		sem_wait(&(cursor->mutex));
		if (cursor->assigned)
			cursor->assigned = FALSE;
		cursor->alive = FALSE;
		temp = cursor;
		cursor = cursor->next;
		sem_post(&(temp->mutex));
		pthread_join(temp->tid, NULL);
		free(temp);
	}
}

void list(){
	char output[256];
	struct artist* cursor = head;
	struct artist* temp;
	while (cursor){
		sem_wait(&(cursor->mutex));
		sprintf(output, "%d %s\n", (int)(cursor->tid), cursor->assigned? "ASSIGNED" : "WAITING");
		cse320_print(output);
		temp = cursor;
		cursor = cursor->next;
		sem_post(&(temp->mutex));
	}
}
