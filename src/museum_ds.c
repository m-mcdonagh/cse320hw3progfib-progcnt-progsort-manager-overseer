#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "museum_overseer.h"
#include "defs.h"
#define BOOLEAN char
#define TRUE 1
#define FALSE 0

struct museum museum_ds[5];
pthread_t gc_tid;
BOOLEAN closing;

void initialize_museum_ds(){
	int i;
	for (i=0; i<5; i++){
		museum_ds[i].numVisitors = 0;
		sem_init(&museum_ds[i].mutex, 0, 1);
		museum_ds[i].open = TRUE;
	}
}

void visitor_in(int n){
	sem_wait(&museum_ds[n-1].mutex);
	museum_ds[n-1].numVisitors++;
	sem_post(&museum_ds[n-1].mutex);
}

void visitor_out(int n){
	sem_wait(&museum_ds[n-1].mutex);
	if (museum_ds[n-1].numVisitors > 0)
		museum_ds[n-1].numVisitors--;
	sem_post(&museum_ds[n-1].mutex);
}

void* in_thread(void* voidptr){
	visitor_in((int)(long)voidptr);
	return NULL;
}

void* out_thread(void* voidptr){
	visitor_out((int)(long)voidptr);
	return NULL;
}

void in_spawner(int n, int x){
	pthread_t* tid = malloc(x * sizeof(pthread_t));
	int i;
	for (i=0; i<x; i++)
		pthread_create(tid + i, NULL, in_thread, (void *)(long)n);
	for (i=0; i<x; i++)
		pthread_join(tid[i], NULL);
	free(tid);
}

void out_spawner(int n, int x){
	pthread_t* tid = malloc(x * sizeof(pthread_t));
	int i;
	for (i=0; i<x; i++)
		pthread_create(tid + i, NULL, out_thread, (void *)(long)n);
	for (i=0; i<x; i++)
		pthread_join(tid[i], NULL);
	free(tid);
}

void museum_info(){
	char msg[32];
	int i;
	for (i=0; i<5; i++){
		sprintf(msg, "%d:%d:%s\n", i+1, museum_ds[i].numVisitors, museum_ds[i].open?"OPEN":"CLOSED");
		cse320_print(msg);
	}
}

void museum_close(int n){
	if (n >= 1 && n <= 5 && museum_ds[n-1].numVisitors == 0)
		museum_ds[n-1].open = FALSE;
	else 
		fprintf(stderr, "Error closing Museum %d.\n", n);
}

void* closing_thread(void* voidptr){
	int i;
	while (closing){
		for (i=1; i<=5; i++){
			sem_wait(&museum_ds[i-1].mutex);
			if (museum_ds[i-1].open && museum_ds[i-1].numVisitors == 0)
				museum_close(i);
			sem_post(&museum_ds[i-1].mutex);
		}
		sleep(3);
	}
	pthread_exit(NULL);
}

void start(){
	pthread_create(&gc_tid, NULL, closing_thread, NULL);
}

void museum_clean(){
	int i;
	for (i=1; i<5; i++){
		sem_wait(&museum_ds[i-1].mutex);
		museum_ds[i-1].numVisitors = 0;
		museum_ds[i-1].open = FALSE;
		sem_post(&museum_ds[i-1].mutex);
	}
	pthread_join(gc_tid, NULL);
}

