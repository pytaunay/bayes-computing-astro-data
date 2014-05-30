#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define NELEM 10000

int main() {

	int *V = (int*)malloc(NELEM*sizeof(int));

	int th_num, num_th;

	#pragma omp parallel private(th_num, num_th)
	{
		th_num = omp_get_thread_num();
		num_th = omp_get_num_threads();

		printf("Hello from thread %d of %d\n",th_num, num_th);
		#pragma omp sections 
		{
			#pragma omp section
			{
				printf("Thread %d of %d -- Section 1\n",th_num, num_th);
				for(int i = 0; i < NELEM; i++)
					V[i] = i;
			}		
			
			#pragma omp section 
			{
				printf("Thread %d of %d -- Section 2\n",th_num, num_th);
			}
		}
	}

	free(V);

	return 0;
}	
	
