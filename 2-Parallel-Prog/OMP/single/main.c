#include <omp.h>
#include <stdio.h>

int main() {
	
	int th_num, num_th;

	#pragma omp parallel private(th_num,num_th)
	{
		th_num = omp_get_thread_num();
		num_th = omp_get_num_threads();

		#pragma omp single
		printf("Single thread number %d of %d\n\n",th_num,num_th);

		printf("Hello from thread %d of %d ! \n",th_num,num_th);
	}

	return 0;
}	
