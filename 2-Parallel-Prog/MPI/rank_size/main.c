#include <mpi.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	
	MPI_Init(&argc, &argv);
	int my_rank, number_proc;

	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &number_proc);

	printf("Hello from process %d of %d\n",my_rank, number_proc);

	MPI_Finalize();

	return 0;
}	
