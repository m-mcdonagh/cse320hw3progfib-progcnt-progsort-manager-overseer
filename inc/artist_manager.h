#ifndef ARTIST_MANAGER
#define ARTIST_MANAGER
#include <pthread.h>

// Defined in artist_ds.c
	struct artist;
	struct artist* newArtist(pthread_t pid);
	void hire(int n);
	void fire(pthread_t x);
	void fireall();
	void assign(pthread_t x);
	void withdraw(pthread_t x);
	void list();

// Defined in artist.c
	void* artistProcess(void* voidptr);

#endif
