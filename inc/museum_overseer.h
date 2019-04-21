#ifndef MUSEUM_OVERSEER
#define MUSEUM_OVERSEER
#include <semaphore.h>
#define BOOLEAN char
#define TRUE 1
#define FALSE 0

struct museum {
	int numVisitors;
	sem_t mutex;
	BOOLEAN open;
};

//defined in museum_ds.c
	void initialize_museum_ds();
	void in_spawner(int n, int x);
	void out_spawner(int n, int x);
	void museum_info();
	void start();
	void museum_clean();

#endif
