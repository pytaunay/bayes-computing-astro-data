#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#define SIZE 2048 

int main() {
	int SIZE_X = SIZE;
	int SIZE_Y = SIZE;

	double tic,toc;

	double *data = (double*)malloc(SIZE_X*SIZE_Y*sizeof(double));

	// Good access
	tic = omp_get_wtime();
	for(int i = 0; i < SIZE_X; i++) {
		for(int j = 0; j < SIZE_Y; j++) {
			data[ j + i*SIZE_Y ] = 10.0 + 3.14159;
		}
	}	
	toc = omp_get_wtime();

	printf("Good access: %f\n",toc-tic);

	// Bad access
	tic = omp_get_wtime();
	for(int i = 0; i < SIZE_X; i++) {
		for(int j = 0; j < SIZE_Y; j++) {
			data[ i + j*SIZE_Y ] = 10.0 + 3.14159;
		}
	}	
	toc = omp_get_wtime();

	printf("Bad access: %f\n",toc-tic);

	free(data);

	return 0;
}	
