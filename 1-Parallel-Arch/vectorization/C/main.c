#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define N 1000000

int main() {

	double *V1 = (double*) malloc(N*sizeof(double));
	double *V2 = (double*) malloc(N*sizeof(double));
	double *V3 = (double*) malloc(N*sizeof(double));
	double *R = (double*) malloc(N*sizeof(double));

	srand(time(NULL));

	for(int i = 0;i<N;i++) {
		V1[i] = (double)i;
		V2[i] = (double)(i)+3.1;
		V3[i] = rand(); 
	}	

	double tic = omp_get_wtime();

	for(int i = 0;i<N;i++)
		R[i] = V1[i] + V2[i]*V3[i];
	
	double toc = omp_get_wtime();

	printf("Total time: %f\n", toc-tic);

	free(R);
	free(V1);
	free(V2);

	return 0;
}	
