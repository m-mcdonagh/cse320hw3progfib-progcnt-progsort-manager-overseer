#ifndef ARTIST_MANAGER
#define ARTIST_MANAGER
#include <pthread.h>
#include <semaphore.h>
#define BOOLEAN char

struct artist{
	pthread_t tid;
	struct artist* next;
	sem_t mutex;
	BOOLEAN assigned;
	BOOLEAN alive;
};

// Defined in artist_ds.c
	struct artist* newArtist();
	void hire(int n);
	void fire(int x);
	void fireall();
	void assign(int x);
	void withdraw(int x);
	void list();

// Defined in artist.c
	void* artistProcess(void* voidptr);

#endif
