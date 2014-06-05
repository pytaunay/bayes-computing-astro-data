#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

// We use omp for accurate timing w/ omp_get_wtime
#include <omp.h>

#include "read_data.h"
#include "log_likelihood.h"
#include "tools.h"

// For convenience
#define COMM MPI_COMM_WORLD

int main(int argc, char *argv[]) {

	// Start MPI
	MPI_Init(&argc,&argv);

	int myrank, nproc = 0;
	MPI_Comm_rank(COMM,&myrank);
	MPI_Comm_size(COMM,&nproc);

	int nobs, sizex, nsample = 0;
	char *location = NULL;
	int ret = 0;

	//////////////////////////////////////
	/////////////// PARSERS //////////////
	//////////////////////////////////////
	// Parse the command line
	int proc_nobs;
	ret = parse_command_line(argc,argv,&nobs,&sizex,&nsample,&location,&proc_nobs,myrank,nproc);
	if( ret != PARSER_SUCCESS ) {
		printf("Parsing failed ! Exiting...\n");
		return EXIT_FAILURE;
	}


	// Allocate the data for ALL processes.
	double *isigma = (double*)malloc(sizex*sizex*sizeof(double));
	double *mu = (double*)malloc(sizex*sizeof(double));
	double det_sigma = 0.0; 

	// Only the first process will parse the data. Others will wait
	double *buffer_X = NULL;
	if( myrank == 0 ) {
		int ret = 0;
		buffer_X = (double*)malloc(nobs*sizex*sizeof(double));	
		ret = read_data(buffer_X, isigma, &det_sigma, mu, &nobs, &sizex, location);
		if( ret != PARSER_SUCCESS ) {
			printf("Parsing failed ! Exiting...\n");
			return EXIT_FAILURE;
		}	
	}
	// Wait for the process 0 to finish parsing the files
	MPI_Barrier(COMM);

	////////////////////////////////////////
	/////////////// Variables //////////////
	////////////////////////////////////////
	// Timing variables
	double tic, toc,tot_time = 0.0; 

	// Sums
	double final_sum = 0.0;
	double my_sum = 0.0;

	// Individual arrays
	double *X = (double*)malloc(proc_nobs*sizex*sizeof(double));	
	double *LV = (double*)malloc(proc_nobs*sizex*sizeof(double)); 
	double *tmp = (double*)malloc(proc_nobs*sizex*sizeof(double)); 
	double *ones = (double*)malloc(proc_nobs*sizeof(double));
	

	////////////////////////////////////////
	/////////////// SEND DATA //////////////
	////////////////////////////////////////
	// Process 0 sends data to everyone with collectives
	MPI_Bcast(isigma,sizex*sizex,MPI_DOUBLE,0,COMM);
	MPI_Bcast(mu,sizex,MPI_DOUBLE,0,COMM);
	MPI_Bcast(&det_sigma,1,MPI_DOUBLE,0,COMM);
	MPI_Scatter(buffer_X,proc_nobs*sizex,MPI_DOUBLE,X,proc_nobs*sizex,MPI_DOUBLE,0,COMM);


	
	////////////////////////////////////////
	/////////////// Algorithm //////////////
	////////////////////////////////////////
	for(int k = 0; k<nsample; k++) {
		tic = omp_get_wtime();


		final_sum = 0.0;
		// Guru approach: BLAS
		log_likelihood(X,isigma,mu,det_sigma,proc_nobs,sizex,&my_sum,LV,tmp,ones);

		// Combine all the intermediary sums in a single one
		MPI_Reduce(&my_sum,&final_sum,1,MPI_DOUBLE,MPI_SUM,0,COMM);
		MPI_Barrier(COMM);
		toc = omp_get_wtime();	
		tot_time += toc-tic;
	}	

	
	////////////////////////////////////////
	/////////////// Results ////////////////
	////////////////////////////////////////
	if( myrank == 0) { 
		printf("Result: %f\n",final_sum);
		printf("Total time (s): %f\n",tot_time/(double)nsample);
	}	


	////////////////////////////////////////
	/////////////// Clean up ///////////////
	////////////////////////////////////////
	if ( myrank == 0 ) {
		free(buffer_X);
	}
	free(X);
	free(isigma);
	free(mu);
	free(location);
	free(LV);
	free(tmp);
	free(ones);

	MPI_Finalize();

	return EXIT_SUCCESS;
}
