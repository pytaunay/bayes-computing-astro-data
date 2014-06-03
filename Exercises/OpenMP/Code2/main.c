#include <stdio.h>
#include <stdlib.h>
#include <omp.h>


#define NELEM 10

int main() {

	int *array = (int*)malloc(NELEM*sizeof(int));
	for(int i = 0; i < NELEM; i++) {
		array[i] = i;
	}

	#pragma omp parallel for
	for(int i = 1; i< NELEM; i++) {
		array[i] = array[i-1];
	}


	for(int i = 0; i < NELEM; i++) {
		printf("array[%d] = %d\n",array[i]);
	}



	return EXIT_SUCCESS;
}	
