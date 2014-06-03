#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <mpi.h>

#define MSIDE 2048 

// Convenience
#define COMM MPI_COMM_WORLD

int main(int argc, char* argv[]) {
	
	MPI_Init(&argc,&argv);

	int myrank = 0;
	int nproc = 1;

	MPI_Comm_rank(COMM,&myrank);
	MPI_Comm_size(COMM,&nproc);

	srand(time(NULL));

	// Allocate a matrix of size MSIDE x MSIDE
	// It will be row major
	double *M = (double*)malloc(MSIDE*MSIDE*sizeof(double));

	// Allocate a vector of size MSIDE x 1
	double *V = (double*)malloc(MSIDE*sizeof(double));
	double *mpi_res = (double*)malloc(MSIDE*sizeof(double));
	double *serial_res = (double*)malloc(MSIDE*sizeof(double));

	// Fill matrix and vector with random data
	if(myrank == 0) {
		for(int i = 0; i < MSIDE; i++) {
			V[i] = (double)rand() / (double)RAND_MAX;
			for(int j = 0; j < MSIDE;j++) {
				M[j+i*MSIDE] = (double)rand() / (double)RAND_MAX;
			}
		}	
	}	
	
	// Calculate the total number of elements to send to each process
	int nrow_S = MSIDE / nproc;
	int nelem_S = MSIDE*nrow_S;

	// Allocate corresponding matrix
	double *S = (double*)malloc(nelem_S*sizeof(double));

	/********************************************************
	//// Question 1: Divide M into submatrices, using MPI collective call
	********************************************************/
	if(myrank == 0) printf("Dividing M...\n");
	MPI_Scatter(M,nelem_S,MPI_DOUBLE,S,nelem_S,MPI_DOUBLE,0,COMM);

	/********************************************************
	//// Question 2: Send V to each process, using MPI collective call
	********************************************************/
	if(myrank == 0) printf("Sending V...\n");
	MPI_Bcast(V,MSIDE,MPI_DOUBLE,0,COMM);

	// Allocate temporary vector result of SxV 
	double *sv_res = (double*)malloc(nrow_S*sizeof(double));
	
	// Perform the submatrix-vector multiplication SxV 
	for(int i = 0; i<nrow_S; i++) {
		sv_res[i] = 0.0;
		for(int j = 0; j<MSIDE;j++) {
			sv_res[i] += S[j+i*MSIDE] * V[j];
		}
	}	

	/********************************************************
	// Question 3: Send the data back to 0 for assembly
	********************************************************/
	if(myrank == 0) printf("Fetching data back...\n");
	MPI_Gather(sv_res,nrow_S,MPI_DOUBLE,mpi_res,nrow_S,MPI_DOUBLE,0,COMM);

	/////////////////////////////
	// L2 Norm		   //
	/////////////////////////////
	// Calculate the square of each element in the subvector sv_res 
	for(int i = 0; i<nrow_S;i++) {
		sv_res[i] *= sv_res[i];
	}	

	// Then sum all the elements in 0: partial sum
	for(int i = 1; i<nrow_S;i++) {
		sv_res[0] += sv_res[i];
	}	

	/********************************************************
	// Question 4: Sum all the squares 
	********************************************************/
	double l2 = 0.0;
	if(myrank == 0) printf("Sum all the squares...\n");

	MPI_Reduce(sv_res,&l2,1,MPI_DOUBLE,MPI_SUM,0,COMM);

	l2 = sqrt(l2);

	if(myrank == 0) {
		printf("L2 norm (MPI): %f\n",l2);
	}	
	

	///////////////////////////////
	// Compare to serial results //
	///////////////////////////////
	if(myrank == 0) {	
		for(int i = 0; i < MSIDE; i++) {
			serial_res[i] = 0.0;
			for(int j = 0; j < MSIDE; j++) {
				serial_res[i] += M[j+i*MSIDE]*V[j];
			}
		}	

		l2 = 0.0;
		for(int i = 0; i < MSIDE; i++) {
			l2 += serial_res[i] * serial_res[i];
		}	
		l2 = sqrt(l2);
		printf("L2 norm (serial): %f\n",l2);
		
	}	

	free(sv_res);
	free(S);
	free(serial_res);
	free(mpi_res);
	free(V);
	free(M);


	MPI_Finalize();

	return EXIT_SUCCESS;
}	
