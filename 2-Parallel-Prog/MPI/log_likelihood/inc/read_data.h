#ifndef _READ_DATA_H
#define _READ_DATA_H

#include <string>

enum {PARSER_FAILED=0,PARSER_SUCCESS=1};

int read_data(	double *buffer_X, 
		double *isigma, 
		double *det_sigma, 
		double *mu, 
		int *nobs, 
		int *sizex, 
		char *location);
	

bool read_file(double *V,std::string &file, int &maxLine); 

#endif

