#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NELEM 10

void print_vector(int *V, int nelem, int rank) {
	printf("Process %d\n",rank);
	for(int i = 0; i < nelem; i++) {
		printf("V[%d] = %d\n",i,V[i]);
	}
}	


int main(int argc, char *argv[]) {
	
	MPI_Init(&argc,&argv);

	int my_rank;
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);

	// Allocate memory: a vector V on rank 0, a vector U on rank 1
	if(my_rank == 0) {

		srand(time(NULL));
		int *V = (int*) malloc(NELEM*sizeof(int));

		for(int i = 0; i < NELEM; i++)
			V[i] = rand(); 

		// Print out content of vector
		print_vector(V,NELEM,my_rank);

		int dest = 1;
		MPI_Send(V,NELEM,MPI_INT,dest,0,MPI_COMM_WORLD);
		printf("New vector V \n");
		print_vector(V,NELEM,my_rank);

		free(V);

	} else if(my_rank == 1) {
		int *U = (int*) malloc(NELEM*sizeof(int));
		// Print out content of vector
		print_vector(U,NELEM,my_rank);

		int src = 0;
		MPI_Status status;
		MPI_Recv(U,NELEM,MPI_INT,src,0,MPI_COMM_WORLD,&status);
		printf("New vector U\n");
		print_vector(U,NELEM,my_rank);

		free(U);
	}

	// Finalize
	MPI_Finalize();

	return 0;
}
			
