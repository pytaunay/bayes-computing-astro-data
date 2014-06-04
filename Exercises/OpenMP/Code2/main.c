#include <stdio.h>
#include <stdlib.h>
#include <omp.h>


#define NELEM 10

int main() {

	int *array = (int*)malloc(NELEM*sizeof(int));
	for(int i = 0; i < NELEM; i++) {
		array[i] = i;
	}
	printf("Array before change:\n");
	for(int i = 0; i < NELEM; i++) {
		printf("array[%d] = %d\n",i,array[i]);
	}

	#pragma omp parallel for
	for(int i = NELEM-1 ; i > 0; i--) {
		array[i] = array[i-1];
	}

	array[0]--;


	printf("Array after change:\n");
	for(int i = 0; i < NELEM; i++) {
		printf("array[%d] = %d\n",i,array[i]);
	}



	return EXIT_SUCCESS;
}	
