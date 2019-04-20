#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include "defs.h"
#include "error_checking.h"

double* array;

struct threadInput{
	int start;
	int end;
};

void* pqsort(void* voidptr){
	struct threadInput* input = (struct threadInput*)voidptr;
	if (input->start >= input->end)
		return NULL;

	double pivot = array[input->end];
	int index = input->start;
	double temp;

	int i;
	for (i=input->start; i<input->end; i++){
		if (array[i] < pivot){
			temp = array[i];
			array[i] = array[index];
			array[index] = temp;
			index++;
		}
	}
	temp = array[input->end];
	array[input->end] = array[index];
	array[index] = temp;
	
	struct threadInput left;
	left.start = input->start;
	left.end = index-1;

	pthread_t tid;
	pthread_create(&tid, NULL, pqsort, (void *)(&left));

	struct threadInput right;
	right.start = index;
	right.end = input->end;

	pqsort((void *)(&right));

	pthread_join(tid, NULL);

	return NULL;
}
	

void main(int argc, char** argv){
	if (argc != 2){
		fprintf(stderr, "ERROR: Invalid Arguments\n");
		exit(EXIT_FAILURE);
	}
	FILE* file = Fopen(*++argv, "r");
	if (!file){
		fprintf(stderr, "ERROR: failed to open \"%s\".\n", *argv);
		exit(EXIT_FAILURE);
	}
	
	char nString[16];
	if (fgets(nString, 16, file) == NULL){
		fprintf(stderr, "ERROR: incorrect file format\n");
		exit(EXIT_FAILURE);
	}
	int n = atoi(nString);
	
	int buffSize = n * 16;
	char* elemList = malloc(buffSize * sizeof(char));
	array = malloc(n * sizeof(double));
	if (fgets(elemList, buffSize, file) == NULL){
		fprintf(stderr, "ERROR: incorrect file format\n");
		exit(EXIT_FAILURE);
	}

	int i;
	char* temp;
	for (i=0; i<n; i++){
		temp = elemList;
		while(*elemList != '\0'){
			elemList++;
			if (isspace(*elemList)){
				*elemList = '\0';
				break;
			}
		}
		elemList++;
		array[i] = atof(temp);
	}
	
	struct threadInput input;
	input.start = 0;
	input.end = n-1;
	pqsort((void *)(&input));

	char buffer[32];
	for(i=0; i<n; i++){
		sprintf(buffer, "%g ", array[i]);
		cse320_print(buffer);
	}
	cse320_print("\n");
	free(array);
		
}
