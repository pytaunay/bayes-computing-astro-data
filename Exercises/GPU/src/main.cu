#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#include <cuda.h>
#include <cublas_v2.h>

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
	double *X = (double*)malloc(nobs*sizex*sizeof(double));	
	double *isigma = (double*)malloc(sizex*sizex*sizeof(double));
	double *mu = (double*)malloc(sizex*sizeof(double));
	double det_sigma = 0.0; 

	ret = read_data(X, isigma, &det_sigma, mu, &nobs, &sizex, location);
	if( ret != PARSER_SUCCESS ) {
		printf("Parsing failed ! Exiting...\n");
		return EXIT_FAILURE;
	}	

	////////////////////////////////////////
	/////////////// Variables //////////////
	////////////////////////////////////////
	// Timing variables
	double tic, toc, tot_time = 0.0; 

	// Result
	double res = 0.0;
	

	// Allocate GPU memory
	double *d_LV, *d_tmp, *d_ones;
	cudaMalloc((void**)&d_LV,nobs*sizex*sizeof(double));
	cudaMalloc((void**)&d_tmp,nobs*sizex*sizeof(double));
	cudaMalloc((void**)&d_ones,nobs*sizeof(double));

	double *d_X, *d_isigma, *d_mu;
	cudaMalloc((void**)&d_X,nobs*sizex*sizeof(double));
	cudaMalloc((void**)&d_isigma,sizex*sizex*sizeof(double));
	cudaMalloc((void**)&d_mu,sizex*sizeof(double));

	// Copy the read data onto the GPU
	cudaMemcpy(d_X,X,nobs*sizex*sizeof(double),cudaMemcpyHostToDevice);
	cudaMemcpy(d_isigma,isigma,sizex*sizex*sizeof(double),cudaMemcpyHostToDevice);
	cudaMemcpy(d_mu,mu,sizex*sizeof(double),cudaMemcpyHostToDevice);

	// Create a handle for cublas, the GPU linear algebra library
	cublasHandle_t handle;
	cublasStatus_t stat;
	stat = cublasCreate(&handle);
	cublasSetPointerMode(handle,CUBLAS_POINTER_MODE_HOST);
	if( stat != CUBLAS_STATUS_SUCCESS ) {
		printf("CUBLAS Initialization failed\n");
		return EXIT_FAILURE;
	}	


	////////////////////////////////////////
	/////////////// Algorithm //////////////
	////////////////////////////////////////
	//// Start time sampling
	for(int k = 0; k < nsample; k++) {
		tic = omp_get_wtime();
		res  = 0.0;

		// Main driver
		log_likelihood(d_X,d_isigma,d_mu,det_sigma,nobs,sizex,&res,d_LV,d_tmp,d_ones,&handle);

		toc = omp_get_wtime();
		tot_time += toc-tic;
	}	

	printf("Result: %f\n",res);
	printf("Total time: %f\n",tot_time/(double)nsample);

	////////////////////////////////////////
	/////////////// Clean up ///////////////
	////////////////////////////////////////
	cudaFree(d_mu);
	cudaFree(d_X);
	cudaFree(d_isigma);
	cudaFree(d_LV);
	cudaFree(d_tmp);
	cudaFree(d_ones);

	free(X);
	free(isigma);
	free(mu);
	free(location);

	return EXIT_SUCCESS;
}
