#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#include "read_data.h"

// Parse all the arguments on the comment line
int parse_command_line(int argc,char *argv[],int *nobs, int *sizex, int *nsample,char **location,int *proc_nobs,int myrank, int nproc) {

	if(argc==-1) {
		if(myrank == 0) printf("Please enter at least one argument\n");
		return PARSER_FAILED;
	}
	else if(argc<8) {
		if(myrank == 0) printf("Usage: -sx Size of the random variable -nobs number of observations -data Path to data file -ns Number of samples\n");
		return PARSER_FAILED;
	}
	else {
		for(int i = 1;i<argc;i+=2) {
			if (!strcmp(argv[i],"-data")) { 
				*location = strdup(argv[i+1]);	
			} else if (!strcmp(argv[i],"-sx")) { 
				*sizex = atoi(argv[i+1]);
			} else if (!strcmp(argv[i],"-nobs")) {
				*nobs= atoi(argv[i+1]);
			} else if (!strcmp(argv[i],"-ns")) {
				*nsample = atoi(argv[i+1]);
			} else { 
				if(myrank == 0) fprintf(stderr,"Argument '%s' non recognized", argv[i]);
				return PARSER_FAILED;
			}	

		}
	}	

	*proc_nobs = *nobs/nproc;

	if(myrank == 0) {
		printf("Arguments:\n");
		printf("Path to data\t%s\n",*location);
		printf("Size of X\t%d\n",*sizex);
		printf("Number of observations\t%d (%d per process)\n",*nobs,*proc_nobs);
		printf("Number of samples\t%d\n",*nsample);
	}	

	return PARSER_SUCCESS;
}
