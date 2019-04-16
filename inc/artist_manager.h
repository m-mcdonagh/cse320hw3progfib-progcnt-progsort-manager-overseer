#ifndef ARTIST_MANAGER
#define ARTIST_MANAGER
#include <sys/types.h>

// Defined in artist_ds.c
	struct artist;
	void newArtist(pid_t pid);
	void hire(int n);
	void fire(pid_t x);
	void fireall();
	void assign(pid_t x);
	void withdraw(pid_t x);
	void list();
	void reapAndRemove(int sig);
	void childFree();

// Defined in artist.c
	void artistProcess();

#endif
