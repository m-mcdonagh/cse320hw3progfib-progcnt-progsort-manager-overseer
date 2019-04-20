#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <ctype.h>
#include "error_checking.h"
#include "defs.h"

struct number_counter{
	int volatile num0s;
	int volatile num1s;
	int volatile num2s;
} *number_counter;

sem_t mutex;

void* threadRoutine(void* voidptr){
	FILE* file = (FILE *) voidptr;
	char nString[16];
	if (fgets(nString, 8, file) == NULL){
		fprintf(stderr, "ERROR: incorrect file format\n");
		exit(EXIT_FAILURE);
	}
	int n = atoi(nString);
	int buffSize = 2 * n + 1;

	char* fileString = malloc(buffSize * sizeof(char));
	char* temp = fileString;

	if (fgets(fileString, buffSize, file) == NULL){
		fprintf(stderr, "ERROR: incorrect file format\n");
		exit(EXIT_FAILURE);
	}
	fclose(file);

	int i;
	int currentInt;
	char currentChar;
	char buf[2];
	for (i=0; i<n; i++){
		currentChar = *fileString;
		sprintf(buf, "%c", currentChar);
		currentInt = atoi(buf);
	
		if (!isdigit(*fileString)){ 
			fprintf(stderr, "ERROR: incorrect file format, 2nd line must contain only %d single digits separated by spaces! Only counted %d number(s)\n", n, i);
			return NULL;
		}

		while (isspace(*++fileString));

		sem_wait(&mutex);
		switch(currentInt){
			case 0:
				(number_counter->num0s)++;
				break;
			case 1:
				(number_counter->num1s)++;
				break;
			case 2:
				(number_counter->num2s)++;
				break;
		}
		sem_post(&mutex);
	}

	free(temp);
	return NULL;
		
}

void main(void){
	number_counter = malloc(sizeof(struct number_counter));
	sem_init(&mutex, 0, 1);

	FILE* file1 = Fopen("file1.dat", "r");
	FILE* file2 = Fopen("file2.dat", "r");
	FILE* file3 = Fopen("file3.dat", "r");

	pthread_t tid[3];

	pthread_create(tid    , NULL, threadRoutine, (void *)file1);
	pthread_create(tid + 1, NULL, threadRoutine, (void *)file2);
	pthread_create(tid + 2, NULL, threadRoutine, (void *)file3);
	
	int i;
	for (i=0; i<3; i++)
		pthread_join(tid[i], NULL);
	
	char output[32];
	sprintf(output, "%d %d %d\n", number_counter->num0s, number_counter->num1s, number_counter->num2s);
	cse320_print(output);

	free(number_counter);
	exit(0);
}

