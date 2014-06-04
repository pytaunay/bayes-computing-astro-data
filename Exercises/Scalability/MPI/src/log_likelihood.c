#include <stdio.h>
#include <mkl.h>
#include <math.h>

#define M_PI 3.14159265358979323846

// Calculates (X-mu)^T * ISIGMA * (X-mu) for nobs random variables of size sizex
// Stores the result in res
// Intemediary vectors LV, tmp, ones are allocated before the function is called
//
// The routine uses BLAS GEMM, GEMV, AXPY, and DDOT for high-perf linear algebra operations
void log_likelihood(double *X, double *isigma, double *mu, double det_sig, const int nobs,const int sizex, double *res, double *LV, double *tmp, double *ones) {

	for(int i = 0; i < nobs; ++i)
		ones[i] = 1.0;
	
	if( nobs > 1) {
		cblas_dgemm(CblasRowMajor,CblasTrans,CblasNoTrans,nobs,sizex,1,1.0,ones,nobs,mu,sizex,0.0,LV,sizex); 
	} else {	
		for(int i = 0; i<sizex; i++) {
			LV[i] = mu[i];
		}
	}

	// Calculate LV = (X-mu)
	// We store mu-X in LV bc. of AXPY: don't want to write over X 
	cblas_daxpy(nobs*sizex,-1.0,X,1,LV,1);
	
	if( nobs > 1 ) {
		// Calculate LV*SIGMA: [NOBS x SIZEX] x [SIZEX x SIZEX]
		cblas_dgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,nobs,sizex,sizex,1.0,LV,sizex,isigma,sizex,0.0,tmp,sizex); 
	} else {
		// If there is only one observation, a gemv is sufficient: 
		// [1 x SIZEX] x [SIZEX x SIZEX]
		cblas_dgemv(CblasRowMajor,CblasTrans,sizex,sizex,1.0,isigma,sizex,LV,1,0.0,tmp,1);
	}	

	// Calculate [LV*SIGMA]*RV = tmp*RV: dot product
	// We use LV as RV, with increments between elements equal to 1, making it a column vector
	double sum = 0.0;
	sum = cblas_ddot(sizex*nobs,tmp,1,LV,1);	

	// Finish 
	sum += (double)nobs*(double)sizex*log(2.0*M_PI) + (double)nobs*log(fabs(det_sig));
	sum *= (-0.5);

	*res = sum;
}
