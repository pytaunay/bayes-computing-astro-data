#include <fstream>
#include <string>
#include <iostream>
#include <vector>

#include <readcsv.h>
#include <read_data.h>


int read_data(	double *buffer_X, 
		double *isigma, 
		double *det_sigma, 
		double *mu, 
		int *nobs, 
		int *sizex, 
		char *location) {

	std::string filename,file = "";
	int nLine = 0;

	// Read the data of each vector
	// MU
	filename = "/mu.csv";
	file = location + filename ;
	nLine = 1;
	
	std::cout << "Parsing MU at " << file << std::endl;
	if( read_file(mu,file,nLine) ) {
		std::cout << "Success" << std::endl;
	} else {
		std::cout << "Failed" << std::endl;
		return PARSER_FAILED;
	}	

	// Read the data of each vector
	// Buffer X
	filename = "/random_vectors.csv";
	file = location + filename ;
	nLine = *nobs;
	
	std::cout << "Parsing X at " << file << std::endl;
	if( read_file(buffer_X,file,nLine) ) {
		std::cout << "Success" << std::endl;
	} else {
		std::cout << "Failed" << std::endl;
		return PARSER_FAILED;
	}	

	// Read the data of each vector
	// Inverse sigma 
	filename = "/inv_sigma.csv";
	file = location + filename ;
	nLine = *sizex;
	
	std::cout << "Parsing SIGMA inverse at " << file << std::endl;
	if( read_file(isigma,file,nLine) ) {
		std::cout << "Success" << std::endl;
	} else {
		std::cout << "Failed" << std::endl;
		return PARSER_FAILED;
	}	

	// Read the data of each vector
	// Determinant of sigma 
	filename = "/det_sigma.csv";
	file = location + filename ;
	nLine = 1;
	
	std::cout << "Parsing SIGMA determinant at " << file << std::endl;
	if( read_file(det_sigma,file,nLine) ) {
		std::cout << "Success" << std::endl;
	} else {
		std::cout << "Failed" << std::endl;
		return PARSER_FAILED;
	}	


	// Print out the data if _VERBOSE_ is defined
	#ifdef _VERBOSE_
	printf("Parsed data\n");
	printf("MU\n");
	for(int i = 0; i<sizex;i++)
		printf("%e,",mu[i]);

	printf("\n");

	printf("X\n");
	for(int i = 0; i<nobs;i++) {
		for(int j = 0; j < sizex;j++) {
			printf("%e,",buffer_X[j+i*sizex]);
		}	
		printf("\n");
	}

	printf("Inverse sigma\n");
	for(int i = 0; i<sizex;i++) {
		for(int j = 0;j<sizex;j++) {
			printf("%e,",isigma[j+i*sizex]);
		}
		printf("\n");
	}	

	printf("Sigma determinant\n");
	printf("%e\n",*det_sigma);
	#endif


	return PARSER_SUCCESS;
}		

bool read_file(double *V, std::string &file, int &maxLine) {

	std::ifstream infile(file);
	if (!infile.is_open()) {
		std::cout << "File " << file << " could not be opened" << std::endl;
		return false;
	}	


	std::string str = "";
	std::vector<double> v;
	int nLine = 0;

	while (getline(infile, str) && nLine < maxLine) {
		if (!client::parse_numbers(str.begin(), str.end(),v) ) {
			std::cout << "Parsing failed\n";
			infile.close(); 
			return false;
		}
		++nLine;
	}

	std::copy(v.begin(),v.end(),V);

	infile.close(); 
	return true;
}
