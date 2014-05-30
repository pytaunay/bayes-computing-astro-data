#include <mpi.h>

#define NELEM 10

int main(int argc, char *argv[]) {

	MPI_Init(&argc, &argv);

	int my_rank;
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);

	int *V = NULL;

	if(my_rank == 0) {
		printf("Hello!\n");
		V = (int*)malloc(NELEM*sizeof(int));
	}

	if(my_rank == 1) {	
		V[0] = 1;
	}	

	MPI_Finalize();

	return 0;
}	
	
