#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <unistd.h>

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


	for(int i = 0; i < nsample; i++) {
		ret = read_data(buffer_X, isigma, &det_sigma, mu, &nobs, &sizex, location);
		if( ret != PARSER_SUCCESS ) {
			printf("Parsing failed ! Exiting...\n");
			return EXIT_FAILURE;
		}	
		sleep(3);
	}	

	////////////////////////////////////////
	/////////////// Clean up ///////////////
	////////////////////////////////////////
	free(buffer_X);
	free(isigma);
	free(mu);
	free(location);

	return EXIT_SUCCESS;
}
