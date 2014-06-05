#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#include "read_data.h"
#include "log_likelihood.h"
#include "tools.h"

int main(int argc, char *argv[]) {

	int nobs, sizex, nsample = 0;
	char *location = NULL;
	int ret = 0;

	//////////////////////////////////////
	/////////////// PARSERS //////////////
	//////////////////////////////////////
	// Parse the command line
	ret = parse_command_line(argc,argv,&nobs,&sizex,&nsample,&location);
	if( ret != PARSER_SUCCESS ) {
		printf("Parsing failed ! Exiting...\n");
		return EXIT_FAILURE;
	}

	// Parse the data on master
	double *buffer_X = (double*)malloc(nobs*sizex*sizeof(double));	
	double *isigma = (double*)malloc(sizex*sizex*sizeof(double));
	double *mu = (double*)malloc(sizex*sizeof(double));
	double det_sigma = 0.0; 

	ret = read_data(buffer_X, isigma, &det_sigma, mu, &nobs, &sizex, location);
	if( ret != PARSER_SUCCESS ) {
		printf("Parsing failed ! Exiting...\n");
		return EXIT_FAILURE;
	}	

	////////////////////////////////////////
	/////////////// Variables //////////////
	////////////////////////////////////////
	// Thread variables
	int nthreads = 1;
	int th_num = 0;
	int th_nobs = nobs;

	nthreads = get_num_threads();


	// Timing variables
	double tic, toc, tot_time = 0.0; 

	//// Arrays for all threads 
	// The pool is allocated inside the shared memory
	double *pool_LV = (double*)malloc(nobs*sizex*sizeof(double)); // Left hand side vector (X-mu)
	double *pool_tmp = (double*)malloc(nobs*sizex*sizeof(double)); // Temporary holder for (X-mu)*SIG 
	double *pool_ones = (double*)malloc(nobs*sizeof(double)); // Temporary holder to create LV
	double *pool_res = (double*)malloc(nthreads*sizeof(double)); // Each thread puts its result in pool_res
	
	// Use pointers to get the correct location in the array 
	double *LV = NULL;
	double *tmp = NULL;
	double *ones = NULL;
	double *X = NULL;

	// Holder for final sum
	double final_sum = 0.0;

	////////////////////////////////////////
	/////////////// Algorithm //////////////
	////////////////////////////////////////
	//// Start time sampling
	for(int k = 0; k < nsample; k++) {
		tic = omp_get_wtime();
		final_sum = 0.0;

		// Main driver
		#pragma omp parallel private(th_num,th_nobs,LV,tmp,ones,X) default(shared)
		{
			// Get thread number
			th_num = omp_get_thread_num();
			// Total number of observations for that thread
			th_nobs = nobs/nthreads;

			// Use the address to point to the correct location in the vector
			X = &buffer_X[th_num*nobs*sizex/nthreads];
			LV = &pool_LV[th_num*th_nobs*sizex];
			tmp = &pool_tmp[th_num*th_nobs*sizex];
			ones = &pool_ones[th_num*th_nobs];
			
			// Each process can now calculate the term in the
			// exponent for a subset of random vectors

			// Naive approach: for loop on each vector X 
			// pool_res[th_num] += exp_term();		

			// Guru approach: BLAS
			log_likelihood(X,isigma,mu,det_sigma,th_nobs,sizex,&pool_res[th_num],LV,tmp,ones);

			#pragma omp barrier

			// Reduction: sum all the intermediary results
			#pragma omp for reduction(+:final_sum)
			for(int i = 0; i < nthreads; i++)
				final_sum = final_sum + pool_res[i];
		}
		toc = omp_get_wtime();
		tot_time += toc-tic;
	}	

	printf("Result: %f\n",final_sum);
	printf("Total time (s): %f\n",tot_time/(double)nsample);

	////////////////////////////////////////
	/////////////// Clean up ///////////////
	////////////////////////////////////////
	free(pool_res);
	free(pool_ones);
	free(pool_tmp);
	free(pool_LV);

	free(buffer_X);
	free(isigma);
	free(mu);
	free(location);

	return EXIT_SUCCESS;
}
