#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
	
	int a = 1;

	#pragma omp parallel default(shared)
	{
		a++;
	}

	printf("a = %d\n",a);

	return EXIT_SUCCESS;
}	
