#include <stdio.h>
#include <math.h>

#include <cuda.h>
#include <cublas_v2.h>

#include <thrust/fill.h>
#include <thrust/device_ptr.h>
#include <thrust/copy.h>

#define M_PI 3.14159265358979323846

// For CUBLAS
#define IDX2C(i,j,ld) (((j)*(ld))+(i))

// Calculates (X-mu)^T * ISIGMA * (X-mu) for nobs random variables of size sizex
// Stores the result in res
// Intemediary vectors LV, tmp, ones are allocated before the function is called
//
// The routine uses the GPU analog of BLAS GEMM, GEMV, AXPY, and DDOT for high-perf linear algebra operations
// The library to do BLAS on the GPU is ... CUBLAS. Note that it is expecting column-major matrices, which changes the different calls to
// GEMM, GEMV, etc.
void log_likelihood(double *X, double *isigma, double *mu, double det_sig, const int nobs,const int sizex, double *res, double *LV, double *tmp, double *ones, cublasHandle_t *handle) {


	cublasStatus_t stat;
	double ZERO = 0.0;
	double ONE = 1.0;
	double NONE = -1.0;

	// Thrust is a library for developer productivity, analog of the C++ standard library
	// Here we define pointers that reside in the device memory: device_ptr
	thrust::device_ptr<double> d_ones = thrust::device_pointer_cast(ones);
	thrust::device_ptr<double> d_mu = thrust::device_pointer_cast(mu);
	thrust::device_ptr<double> d_LV = thrust::device_pointer_cast(LV);
	thrust::device_ptr<double> d_tmp = thrust::device_pointer_cast(tmp);

	// We can them conveniently use them through readily available routines such as fill, copy, transform, etc
	// Fill "ones" with 1
	thrust::fill(d_ones, d_ones+nobs, ONE);

	if( nobs > 1) {
	//	cblas_dgemm(CblasRowMajor,CblasTrans,CblasNoTrans,nobs,sizex,1,1.0,ones,nobs,mu,sizex,0.0,LV,sizex); 
		stat = cublasDgemm(*handle,CUBLAS_OP_N,CUBLAS_OP_T,sizex,nobs,1,&ONE,mu,sizex,ones,nobs,&ZERO,LV,sizex); 
		if( stat != CUBLAS_STATUS_SUCCESS ) {
			printf("CUBLAS DGEMM failed\n");
			exit(0);
		}	
	} else {	
		thrust::copy(d_mu,d_mu+sizex,d_LV);
	}

	// Calculate LV = (X-mu)
	// We store mu-X in LV bc. of AXPY: don't want to write over X 
	// cblas_daxpy(nobs*sizex,-1.0,X,1,LV,1);
	stat = cublasDaxpy(*handle,nobs*sizex,&NONE,X,1,LV,1);
	if( stat != CUBLAS_STATUS_SUCCESS ) {
		printf("CUBLAS DAXPY failed\n");
		exit(0);
	}	
	
	if( nobs > 1 ) {
		// Calculate LV*SIGMA: [NOBS x SIZEX] x [SIZEX x SIZEX]
		//cblas_dgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans,nobs,sizex,sizex,1.0,LV,sizex,isigma,sizex,0.0,tmp,sizex); 
		stat = cublasDgemm(*handle,CUBLAS_OP_T,CUBLAS_OP_N,sizex,nobs,sizex,&ONE,isigma,sizex,LV,sizex,&ZERO,tmp,sizex);
		if( stat != CUBLAS_STATUS_SUCCESS ) {
			printf("CUBLAS DGEMM failed\n");
			exit(0);
		}	
	} else {
		// If there is only one observation, a gemv is sufficient: 
		// [1 x SIZEX] x [SIZEX x SIZEX]
		// cblas_dgemv(CblasRowMajor,CblasTrans,sizex,sizex,1.0,isigma,sizex,LV,1,0.0,tmp,1);
		stat = cublasDgemv(*handle,CUBLAS_OP_T,sizex,sizex,&ONE,isigma,sizex,LV,1,&ZERO,tmp,1);
		if( stat != CUBLAS_STATUS_SUCCESS ) {
			printf("CUBLAS DGEMV failed\n");
			exit(0);
		}	
	}	

	// Calculate [LV*SIGMA]*RV = tmp*RV: dot product
	// We use LV as RV, with increments between elements equal to 1, making it a column vector
	double sum = 0.0;
	// sum = cblas_ddot(sizex*nobs,tmp,1,LV,1);	
	stat = cublasDdot(*handle,sizex*nobs,tmp,1,LV,1,&sum);
	if( stat != CUBLAS_STATUS_SUCCESS ) {
		printf("CUBLAS DDOT failed: %d\n",stat);
		exit(0);
	}	

	// Finish 
	sum += (double)nobs*(double)sizex*log(2.0*M_PI) + (double)nobs*log(fabs(det_sig));
	sum *= (-0.5);

	*res = sum;
}

