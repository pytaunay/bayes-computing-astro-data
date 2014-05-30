#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define NELEM 10

int main() {
	int *a = (int*)malloc(NELEM*sizeof(int));
	int *b = (int*)malloc(NELEM*sizeof(int));
	int *c = (int*)malloc(NELEM*sizeof(int));
	int *x = (int*)malloc(NELEM*sizeof(int));
	int *y = (int*)malloc(NELEM*sizeof(int));
	int *z = (int*)malloc(NELEM*sizeof(int));

	#pragma omp parallel shared(a,b,c,x,y,z)
	{
		#pragma omp for nowait
		for(int i = 0; i < NELEM; i++)
			a[i] = b[i] + c[i];

		#pragma omp for
		for(int i = 0; i < NELEM; i++)
			z[i] = a[i]*x[i] + y[i];
		
	}

	free(a);
	free(b);
	free(c);
	free(x);
	free(y);
	free(z);

	return EXIT_SUCCESS;
}	
